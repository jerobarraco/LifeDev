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
#include "Inventory/InventoryMan.h"
#include "Story/StoryMan.h"
#include "Story/Story.h"
#include "Sounds/MusicMan.h"
#include "Diags/Diags.h"
#include "Engine/PostProcessVolume.h"
#include "JSig/CSignificance.h"
#include "JUtils/Misc/JUtilsMisc.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
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
#include "LifeDev/Game/Dialogs/LDiagMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Flashback/FlashbackMan.h"
#include "LifeDev/Game/Inventory/LInventoryMan.h"

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
	UCInteract::SetDefaultCollisionProfile(UCInteract::ProfileInteract);
}

bool ALGGameMode::LoadChapter() {
	const ULSysSettings* const SysSettings = ULSysSettings::Get();
	const UDataTable* const DT_Chaps = SysSettings->Chapters.LoadSynchronous();
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

void ALGGameMode::BeginPlay() {
	Super::BeginPlay();
	
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;
	
	const ULGameInstance* const Instance = Cast<ULGameInstance>(GetGameInstance());
	if (UNLIKELY(!IsValid(Instance))) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs No valid instance found"), __func__);
		return;
	}
	
	Settings = Instance->GetSubsystem<ULSettings>();
	if (UNLIKELY(!IsValid(Settings))) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Settings not valid. can't continue. S T O P."), __func__);
		return;
	}

	// load the save and init. note: the init is blocked until the save is available since we really
	// need that beforehand. and can't work reliably without it.
	// all the important objects are also spawned dynamically and not set in world, that gives us more control.
	// - Thank you so much Jero, that's really how i needed it.
	// - dou itashimashite!
	if (UNLIKELY(!IsValid(Settings->Save))) {
		UE_LOG(LogLGameMode, Log, TEXT("%hs Savegame not valid. Attempt to load or create."), __func__);
		Settings->OnSaving.AddUniqueDynamic(this, &ALGGameMode::InitOnSave);
		Settings->Init(); // force load. if it's currently loading then it won't re-trigger
		return;
	}
	
	if (UNLIKELY(Settings->GetIsSaving())) {
		Settings->OnSaving.AddUniqueDynamic(this, &ALGGameMode::InitOnSave);
		UE_LOG(LogLGameMode, Log, TEXT("%hs Savegame currently loading. waiting for it."), __func__);
		return;
	}
	
	UE_LOG(LogLGameMode, Log, TEXT("%hs Savegame seems loaded."), __func__);

	// manually go to init if it's already loaded.
	FTimerManager& Timer = World->GetTimerManager();
	Timer.SetTimerForNextTick(this, &ALGGameMode::Init);
}

void ALGGameMode::InitOnSave(const bool IsBusy) {
	if (LIKELY(!IsBusy)) Init();
}

void ALGGameMode::Init() {
	// this is the place were we are going to be initializing everything.
	// the savegame should be already loaded.
	Settings->OnSaving.RemoveAll(this);

	UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	const ULSysSettings* const SysSettings = ULSysSettings::Get();
	if (UNLIKELY(!IsValid(SysSettings))) {
		UE_LOG(LogLGameMode, Warning, TEXT("System Settings not valid. can't continue."));
		return;
	}

	////  subsystems
	// start by initializing the subsystems, since most other stuff needs it.
	
	/// Dialogs
	Diags = World->GetSubsystem<UDiags>();
	if (UNLIKELY(!Diags)) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Can't get the Diags subsystem."), __func__);
		return;
	}

	/// Inventory
	Flags = World->GetSubsystem<UFlags>();
	if (UNLIKELY(!Flags)){
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Can't get the Diags subsystem."), __func__);
		return;
	}
	
	Inventory = World->GetSubsystem<UInventory>();
	if (UNLIKELY(!Inventory)) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Can't get the Diags subsystem."), __func__);
		return;
	}
	
	Story = World->GetSubsystem<UStory>();
	if (UNLIKELY(!Story)) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Can't get the Diags subsystem."), __func__);
		return;
	}

	const bool IsEditor = UJUtilsMisc::IsEditor();
	Story->FadeTime = IsEditor ? 1: FadeTime;
	Story->HoldTime = IsEditor ? 1: HoldTime;

	// init together. but before writing subsystems from save
	Inventory->Init(SysSettings->Inventory.LoadSynchronous());
	Flags->Init();
	Diags->Init();
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

	InventoryMan = Cast<ALInventoryMan>(World->SpawnActor(ALInventoryMan::StaticClass()));
	if (LIKELY(IsValid(InventoryMan))) {
		// goes below the dialogs. because some items will trigger a dialog.
		InventoryMan->InputPrio = 9;
		InventoryMan->ZOrder = 1;
		InventoryMan->Init();
	} else
		InventoryMan = nullptr;

	StoryMan = Cast<ALStoryMan>(World->SpawnActor(ALStoryMan::StaticClass()));
	if (LIKELY(IsValid(StoryMan))) {
		StoryMan->ZOrder = 5;
		StoryMan->Init();
	} else
		StoryMan = nullptr;

	DiagMan = Cast<ALDiagMan>(World->SpawnActor(ALDiagMan::StaticClass()));
	if (LIKELY(IsValid(DiagMan))) {
		// Needs to be 10 so that it takes precedence over the character
		DiagMan->InputPrio = 10;
		DiagMan->ZOrder = 3;
		DiagMan->DebugSkip = !Settings->GetFeat(EFeat::D_SHOW); // skip dialogs if no feature for it
		DiagMan->Init();
	} else
		DiagMan = nullptr;

	MusicMan = Cast<ALMusicMan>(World->SpawnActor(ALMusicMan::StaticClass()));
	if (LIKELY(MusicMan)) MusicMan->Init();

	FlashbackMan = Cast<AFlashbackMan>(World->SpawnActor(AFlashbackMan::StaticClass()));
	if (LIKELY(FlashbackMan)) FlashbackMan->Init();

	// do at the end since it depends on other things.
	// will race-condition the ghosts
	FeatsMan = Cast<ALFeatsMan>(World->SpawnActor(ALFeatsMan::StaticClass()));
	if (LIKELY(FeatsMan)) FeatsMan->Init();

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
	if (LIKELY(IsValid(Char))) {
		Char->InputPrio = 1;
		Char->Init();
	} else Char = nullptr;
	
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

void ALGGameMode::DeInit() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;
	World->GetTimerManager().ClearAllTimersForObject(this);
	
	if (LIKELY(IsValid(Diags))) {
		Diags->OnShow.RemoveAll(this);
		Diags->OnDone.RemoveAll(this);
		Diags->DeInit();
	}
	Diags = nullptr;

	if (LIKELY(IsValid(Inventory))) Inventory->DeInit();
	Inventory = nullptr;
	
	if (LIKELY(IsValid(Flags))) Flags->DeInit();
	Flags = nullptr;
	
	if (LIKELY(IsValid(DiagMan))) DiagMan->DeInit();
	DiagMan = nullptr;

	if (IsValid(InventoryMan)) InventoryMan->DeInit();
	InventoryMan = nullptr;

	if (LIKELY(IsValid(StoryMan))) StoryMan->DeInit();
	StoryMan = nullptr;

	if (LIKELY(IsValid(Story))) {
		Story->OnSeqStop.RemoveAll(this);
		Story->OnFade.RemoveAll(this);
	}
	Story = nullptr;

	// if (IsValid(Char)) Char->DeInit();
	Char = nullptr;

	// probably won't get a chance to fade since the game mode is ending. but for sake of completion.
	if (LIKELY(IsValid(MusicMan))) MusicMan->Fade(false);
	MusicMan = nullptr;
	
	FlashbackMan = nullptr;
	Settings = nullptr; // no deinit. it's a gameinstance subystem
}

void ALGGameMode::SetCharInputEnabled(const bool Enabled) {
	UE_LOG(LogLGameMode, Log, TEXT("%hs. Enabled=%i"), __func__, Enabled);
	CharInputEnabled = Enabled;
	SetTempInputEnabled(Enabled);
}

void ALGGameMode::SetTempInputEnabled(const bool Enabled) const {
	UE_LOG(LogLGameMode, Log, TEXT("%hs. Enabled=%i"), __func__, Enabled);
	if (Enabled && !CharInputEnabled) return;

	if (LIKELY(IsValid(Char))) Char->SetInputEnabled(Enabled);
	if (LIKELY(IsValid(InventoryMan))) InventoryMan->SetVisible(Enabled);
}

ALGGameMode* ALGGameMode::Instance(const UObject* const O) {
	const UWorld* const World = O? O->GetWorld(): nullptr;
	if (UNLIKELY(!IsValid(World))) return nullptr;

	AGameModeBase* const AuthGameMode = World->GetAuthGameMode();
	if (UNLIKELY(!IsValid(AuthGameMode))) return nullptr;
	
	ALGGameMode* const LGGameMode = Cast<ALGGameMode>(AuthGameMode);
	if (UNLIKELY(!IsValid(LGGameMode))) return nullptr;

	return LGGameMode;
}

void ALGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DeInit();
	Super::EndPlay(EndPlayReason);
}

void ALGGameMode::StartChapter() {
	const int32 ChapterId = Settings->CurrentChapter();
	const EFeat& ChapFeat = Settings->CurrentChapterFeat();

	UE_LOG(LogLGameMode, Log, TEXT("%hs Attempting to start chapter id=%i feat=%s"),
		__func__, ChapterId, *UEnum::GetValueAsString(ChapFeat));
	const ULGameInstance* const Instance = Cast<ULGameInstance>(GetGameInstance());
	if (UNLIKELY(!IsValid(Instance) || !IsValid(Story))) {
		// Should this be here?
		UE_LOG(LogLGameMode, Warning, TEXT("%hs No game instance or story or story manager. Can't proceed."),
			__func__);
		return;
	}

	// stop here to avoid getting the engine stuck trying to load chapters
	if (UNLIKELY(ChapFeat == EFeat::C_MAX)) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Went beyond available chapters. Stopping dry. id=%i."),
			__func__, ChapterId);
		return;
	}
	
	// skip chapter if not enabled or just started
	if (UNLIKELY(ChapFeat == EFeat::NONE || !Settings->GetFeat(ChapFeat))) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Skipping chapter. Not in game Feats. id=%i."),
			__func__, ChapterId);
		StartNextChapter(); // note this is recursive but there ain't that many chapters
		return;
	}

	/// finishing previous one
	// save the last played chapter
	Settings->SaveGame();

	/// load new one
	if (UNLIKELY(!LoadChapter())) {
		UE_LOG(LogLGameMode, Warning, TEXT("%hs Chapter didn't load. Won't start any sequence."), __func__);
		return;
	}

	// the story manager will make the gm disable/enable the input
	// Start the sequence.
	Story->StartSequence(Chapter.Steps);
	MusicMan->SetEnviron(true);
}

void ALGGameMode::StartNextChapter() {
	if (UNLIKELY(!IsValid(Settings->Save))) {
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
	if (UNLIKELY(IsFirstFade)) {
		IsFirstFade = false;
		if (LIKELY(StoryMan)) StoryMan->ShowBGSolid(false);
	}

	// fading in requires a timer.
	const float Wait = (Story->FadeTime)+Story->HoldTime;
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	FTimerManager& Time = World->GetTimerManager();
	FTimerHandle Handle2;
	Time.SetTimer(Handle2, this, &ALGGameMode::SetInputEnable, Wait, false);
}

void ALGGameMode::TickCounter() const {
	// while these DO work. they now spam the console with 2 "exec commands"
	// GEngine->Exec(nullptr, TEXT("log LogFlags off"));
	// GEngine->Exec(nullptr, TEXT("log LogFlags on"));
	if (LIKELY(Flags)) Flags->Mod(LDConsts::Flags::Stats::TimeUsed, CounterTime, false);
}
