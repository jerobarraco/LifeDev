// Copyright Jeronimo Barraco-Marmol

#include "LGGameMode.h"

#include "CoreGlobals.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"


#include "Interact/CInteract.h"
#include "Inventory/Inventory.h"
#include "Interact/CInteractor.h"
#include "Interact/Animator/CAnimator.h"
#include "Inventory/Flags.h"
#include "Inventory/InventoryManager.h"
#include "Story/StoryManager.h"
#include "Story/Story.h"
#include "Sounds/MusicMan.h"
#include "Diags/Diags.h"
#include "Engine/PostProcessVolume.h"
#include "JSig/CSignificance.h"
#include "JUtils/Misc/JUtilsMisc.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Consts/ConstSettings.h"
#include "LifeDev/Core/LGameInstance.h"
#include "LifeDev/Core/Settings/FLChapter.h"
#include "LifeDev/Core/Settings/LFeatsMan.h"
#include "LifeDev/Core/Settings/LSave.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Core/Story/LStep.h"
#include "LifeDev/Core/Story/LStoryMan.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Char/LGPController.h"
#include "LifeDev/Game/Dialogs/LDialogMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Flashback/FlashbackMan.h"
#include "LifeDev/Game/Inventory/LInventoryManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogLGameMode, Log, Log);

static float FadeTime = 2;
static float HoldTime = 2;

ALGGameMode::ALGGameMode():Super() {
	Super::SetActorTickEnabled(false);
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<ALChar>
		CChar(TEXT("/Game/LifeDev/Game/Char/LChar_B"));
	DefaultPawnClass = CChar.Succeeded() ? CChar.Class.Get(): ALChar::StaticClass();

	PlayerControllerClass = ALGPController::StaticClass();
	
	// UCInteractor::SetCollisionChannel(InteractTraceChannel);
	// not using the interact trace channel
	// since i want things like walls to block it,
	// without having to set it up for every single mesh.
	// Setting it to visibility has its downsides, like having to set up more complicated collisions
	UCInteractor::SetCollisionChannel(ECC_Visibility);
	// UCInteract::CollisionProfile = "BlockAllDynamic";
	UCInteract::SetCollisionProfile("Interact");
	// UCInteract::CollisionProfile = "Interact";
}

bool ALGGameMode::LoadChapter() {
	ULSysSettings* const SysSettings = ULSysSettings::Get();
	UDataTable* const DT_Chaps = SysSettings->Chapters.LoadSynchronous();
	if (!IsValid(DT_Chaps)) {
		UE_LOG(LogLGameMode, Warning, TEXT("Chapter Datatable is not properly set in the settings."));
		return false;
	}

	// load a chapter based on the rowname. which is just an int to string of the chapter id.
	// todo find a betterest way
	const FName ChapName = *FString::FromInt(Settings->CurrentChapter());
	FLChapter* const pChap = DT_Chaps->FindRow<FLChapter>(ChapName, TEXT(""));
	if (!pChap) {
		UE_LOG(LogLGameMode, Warning, TEXT("Can't get the chapter from datatable. Row=%s."), *ChapName.ToString());
		return false;
	}

	Chapter = *pChap; // Make a copy
	// set them on the dialog subsystem
	UDataTable* const Chars = SysSettings->Characters.LoadSynchronous();
	UDataTable* const DiagData = Chapter.Dialogs.LoadSynchronous();
	UDataTable* const Seqs = Chapter.Sequences.LoadSynchronous();
	Diags->SetData(DiagData, Chars, Seqs);
	return true;
}

void ALGGameMode::Init_Implementation() {
	// this is the place were we are going to be initializing everything.
	// the savegame should be already loaded.
	Settings->OnSaveReady.RemoveAll(this);

	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	const ULSysSettings* const SysSettings = ULSysSettings::Get();
	if (!IsValid(SysSettings)) {
		UE_LOG(LogLGameMode, Warning, TEXT("System Settings not valid. can't continue."));
		return;
	}

	////  subsystems
	// start by initializing the subsystems, since most other stuff needs it.
	
	/// Dialogs
	Diags = World->GetSubsystem<UDiags>();
	if (!Diags) return; // TODO log 
	Diags->Init();

	/// Inventory
	Flags = World->GetSubsystem<UFlags>();
	if (!Flags) return; // TODO log 
	Flags->Init();
	Inventory = World->GetSubsystem<UInventory>();
	Inventory->Init(SysSettings->Inventory.LoadSynchronous());

	Story = World->GetSubsystem<UStory>();
	if (!Story) return; // TODO log 
	const bool IsEditor = UJUtilsMisc::IsEditor();
	Story->FadeTime = IsEditor ? 1: FadeTime;
	Story->HoldTime = IsEditor ? 1: HoldTime;
	Story->Init();

	// now load the values from the save
	// ensure the save-game loads the data into the subsystems.
	// do only after subsystems have been initialized.
	// do before StartChapter since that saves the gamefile (loading from subsystems)
	Settings->Save->WriteSubsystems(World);

	///~ Subs-init finished.

	// set flags, the feats are dependent on the savegame and subsystems
	UCAnimator::Debug = Settings->GetFeat(EFeat::DBG_ANIMS);
	UFlashback::Debug = Settings->GetFeat(EFeat::DBG_FB);
	AStep::Debug = Settings->GetFeat(EFeat::DBG_STEPS);
	UCSignificance::Debug = Settings->GetFeat(EFeat::DBG_SIG);
	
	/// Managers
	// now the managers. which, as they are actors they tend to have side-effects, some of which requires the subsystems

	InventoryMan = Cast<ALInventoryManager>(World->SpawnActor(ALInventoryManager::StaticClass()));
	if (IsValid(InventoryMan)) {
		// goes below the dialogs. because some items will trigger a dialog.
		InventoryMan->InputPrio = 9;
		InventoryMan->ZOrder = 1; 
		InventoryMan->Init();
	} else
		InventoryMan = nullptr;

	StoryMan = Cast<ALStoryMan>(World->SpawnActor(ALStoryMan::StaticClass()));
	if (IsValid(StoryMan)) {
		StoryMan->ZOrder = 5;
		StoryMan->Init();
	} else
		StoryMan = nullptr;

	DiagMan = Cast<ALDialogMan>(World->SpawnActor(ALDialogMan::StaticClass()));
	if (IsValid(DiagMan)) {
		// Needs to be 10 so that it takes precedence over the character
		DiagMan->InputPrio = 10;
		DiagMan->ZOrder = 3; 
		DiagMan->DebugSkip = !Settings->GetFeat(EFeat::D_SHOW); // skip dialogs if no feature for it
		DiagMan->Init();
	} else
		DiagMan = nullptr;

	// TODO add an init to these. to avoid the race conditions i had with the ghosts
	MusicMan = Cast<ALMusicMan>(World->SpawnActor(ALMusicMan::StaticClass()));
	FlashbackMan = Cast<AFlashbackMan>(World->SpawnActor(AFlashbackMan::StaticClass()));
	// do at the end since it depends on other things.
	FeatsMan = Cast<ALFeatsMan>(World->SpawnActor(ALFeatsMan::StaticClass()));
	// will race-condition the ghosts

	/// GameMode init starts
	
	// post process (does this even works?)
	PostProcess = Cast<APostProcessVolume>(
		UGameplayStatics::GetActorOfClass(World, APostProcessVolume::StaticClass()));

	/// set input mode
	// this is critical or the dialogs will break
	APlayerController* const Controller = UGameplayStatics::GetPlayerController(World, 0);
	// these are not needed since we are using the input actions
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);
	Controller->bShowMouseCursor = false;

	// Character
	Char = Cast<ALChar>(UGameplayStatics::GetActorOfClass(World, ALChar::StaticClass()));
	if (IsValid(Char)) Char->InputPrio = 1; // Char->Init(); // TODO add char init after the subs. so that the foxification works
	else Char = nullptr;
	
	// start listening only here. in case the previous init might trigger a false one
	Diags->OnShow.AddUniqueDynamic(this, &ALGGameMode::DiagShown);
	Diags->OnDone.AddUniqueDynamic(this, &ALGGameMode::DiagDone);
	Story->OnSeqStop.AddUniqueDynamic(this, &ALGGameMode::StartNextChapter);
	Story->OnFade.AddUniqueDynamic(this, &ALGGameMode::Fade);

	FTimerManager& Timer = World->GetTimerManager();
	// force the input disabled. even though the story manager will make this disable later.
	// in case something goes wrong.
	// disable input on next tick to avoid a crash otherwise....
	Timer.SetTimerForNextTick(this, &ALGGameMode::SetInputDisable);

	FTimerHandle Handle;
	// wait for loading. then start the story!
	Timer.SetTimer(Handle, this, &ALGGameMode::StartChapter, .1);
	Timer.SetTimer(CounterHandle, this, &ALGGameMode::TickCounter, CounterTime, true);
}

void ALGGameMode::BeginPlay() {
	Super::BeginPlay();
	
	const UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	
	const ULGameInstance* const Instance = Cast<ULGameInstance>(GetGameInstance());
	if (!IsValid(Instance)) {
		UE_LOG(LogLGameMode, Warning, TEXT("No valid instance found"));
		return;
	}
	
	Settings = Instance->GetSubsystem<ULSettings>();
	if (!IsValid(Settings)) {
		UE_LOG(LogLGameMode, Warning, TEXT("Settings not valid. can't continue."));
		return;
	}

	// load the save and init. note: the init is blocked until the save is available since we really
	// need that beforehand. and can't work reliably without it.
	// all the important objects are also spawned dynamically and not set in world, that gives us more control.
	// - Thank you so much Jero, that's really how i needed it.
	// - dou itashimashite!
	if (!IsValid(Settings->Save)) {
		UE_LOG(LogLGameMode, Warning, TEXT("Savegame not valid. Attempt to load or create."));
		Settings->OnSaveReady.AddUniqueDynamic(this, &ALGGameMode::Init);
		Settings->Init(); // force load. if it's currently loading then it won't re-trigger
		return;
	}
	if (Settings->GetIsSaving()) {
		Settings->OnSaveReady.AddUniqueDynamic(this, &ALGGameMode::Init);
		UE_LOG(LogLGameMode, Warning, TEXT("Savegame currently loading. waiting for it."));
		return;
	}
	
	UE_LOG(LogLGameMode, Warning, TEXT("Savegame seems loaded."));

	// manually go to init if it's already loaded.
	FTimerManager& Timer = World->GetTimerManager();
	Timer.SetTimerForNextTick(this, &ALGGameMode::Init);
}

void ALGGameMode::DeInit_Implementation() {
	const UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	World->GetTimerManager().ClearAllTimersForObject(this);
	
	if (IsValid(Diags)) {
		Diags->OnShow.RemoveAll(this);
		Diags->OnDone.RemoveAll(this);
		Diags->DeInit();
	}
	Diags = nullptr;

	if (IsValid(Inventory)) Inventory->DeInit();
	Inventory = nullptr;
	
	if (IsValid(Flags)) Flags->DeInit();
	Flags = nullptr;
	
	if (IsValid(DiagMan)) DiagMan->DeInit();
	DiagMan = nullptr;

	if (IsValid(InventoryMan)) InventoryMan->DeInit();
	InventoryMan = nullptr;

	if (IsValid(StoryMan)) StoryMan->DeInit();
	StoryMan = nullptr;

	if (IsValid(Story)) {
		Story->OnSeqStop.RemoveAll(this);
		Story->OnFade.RemoveAll(this);
	}
	Story = nullptr;

	// if (IsValid(Char)) Char->DeInit();
	Char = nullptr;

	// probably won't get a chance to fade since the game mode is ending. but for sake of completion.
	if (IsValid(MusicMan)) MusicMan->Fade(false);
	MusicMan = nullptr;
	
	FlashbackMan = nullptr;
	Settings = nullptr; // no deinit. it's a gameinstance subystem
}

void ALGGameMode::SetCharInputEnabled(bool Enabled) {
	UE_LOG(LogLGameMode, Log, TEXT("%hs. Enabled=%i"), __func__, Enabled);
	CharInputEnabled = Enabled;
	SetTempInputEnabled(Enabled);
}

void ALGGameMode::SetTempInputEnabled(bool Enabled) {
	UE_LOG(LogLGameMode, Log, TEXT("%hs. Enabled=%i"), __func__, Enabled);
	if (Enabled && !CharInputEnabled) return;
	
	if(IsValid(Char)) Char->SetInputEnabled(Enabled);
	if (IsValid(InventoryMan)) InventoryMan->SetVisible(Enabled);
}

ALGGameMode* ALGGameMode::Instance(const UObject* const O) {
	const UWorld* const World = O? O->GetWorld(): nullptr;
	if (!IsValid(World)) return nullptr;

	AGameModeBase* const AuthGameMode = World->GetAuthGameMode();
	if (!IsValid(AuthGameMode)) return nullptr;
	
	ALGGameMode* const LGGameMode = Cast<ALGGameMode>(AuthGameMode);
	if (!IsValid(LGGameMode)) return nullptr;

	return LGGameMode;
}

void ALGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DeInit();
	Super::EndPlay(EndPlayReason);
}

void ALGGameMode::StartChapter() {
	const int32 ChapterId = Settings->CurrentChapter();

	UE_LOG(LogLGameMode, Log, TEXT("Attempting to start chapter id=%i"), ChapterId);
	const ULGameInstance* const Instance = Cast<ULGameInstance>(GetGameInstance());
	if (!IsValid(Instance) || !IsValid(Story)) {
		// Should this be here?
		UE_LOG(LogLGameMode, Warning, TEXT("No game instance or story or story manager. Can't proceed."));
		return;
	}

	// stop here to avoid getting the engine stuck trying to load chapters
	if (ChapterId >= LDConsts::Feats::ChapFeatN) {
		UE_LOG(LogLGameMode, Warning, TEXT("Went beyond available chapters. Stopping dry. id=%i."), ChapterId);
		return;
	}
	
	// skip chapter if not enabled
	if (ChapterId <0 || !Settings->GetFeat(LDConsts::Feats::ChapFeats[ChapterId])) {
		UE_LOG(LogLGameMode, Warning, TEXT("Skipping chapter. Not in game Feats. id=%i."), ChapterId);
		StartNextChapter(); // note this is recursive but there ain't that many chapters
		return;
	}

	/// finishing previous one
	// save the last played chapter
	Settings->SaveGame();

	/// load new one
	if (!LoadChapter()) {
		UE_LOG(LogLGameMode, Warning, TEXT("Chapter didn't load. Won't start any sequence."));
		return;
	}

	// the story manager will make the gm disable/enable the input
	// Start the sequence.
	Story->StartSequence(Chapter.Steps);
	MusicMan->SetEnviron(true);
}

void ALGGameMode::StartNextChapter() {
	if (!IsValid(Settings->Save)) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs: Savegame is null. can't progress."), __func__);
		return;
	}

	// can't remember if this happens during the fade out. but i'm confident i would have coded it that way.
	// clean the ghosts
	MusicMan->KillGhosts();
	
	// Chapter done. go to the next one.
	Settings->Save->ChapterID++;
	StartChapter();
}

void ALGGameMode::DiagShown(const FDialog& Diag) {
	SetTempInputEnabled(false);
}

void ALGGameMode::DiagDone() {
	SetTempInputEnabled(true);
}

void ALGGameMode::Fade(const bool bIn, const FText& Text) {
	if (!bIn) {
		SetInputDisable();
		return;
	}

	// remove the blinds if it's the 1st time.
	if (IsFirstFade) {
		IsFirstFade = false;
		if (StoryMan) StoryMan->ShowBGSolid(false);
	}

	// fading in requires a timer.
	const float Wait = (Story->FadeTime)+Story->HoldTime;
	const UWorld* const World = GetWorld();
	if (!World) return;

	FTimerManager& Time = World->GetTimerManager();
	FTimerHandle Handle2;
	Time.SetTimer(Handle2, this, &ALGGameMode::SetInputEnable, Wait, false);
}

void ALGGameMode::TickCounter() const {
	// while these DO work. they now spam the console with 2 "exec commands"
	// GEngine->Exec(nullptr, TEXT("log LogFlags off"));
	// GEngine->Exec(nullptr, TEXT("log LogFlags on"));
	if (Flags) Flags->Mod(LDConsts::Flags::Stats::TimeUsed, CounterTime, false);
}
