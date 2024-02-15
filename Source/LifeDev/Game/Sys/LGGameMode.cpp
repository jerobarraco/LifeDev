// Copyright Jeronimo Barraco-Marmol

#include "LGGameMode.h"

#include "CoreGlobals.h"
#include "CoreGlobals.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Consts/ConstSettings.h"
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
#include "JUtils/JMiscUtils.h"

#include "LifeDev/Core/LGameInstance.h"
#include "LifeDev/Core/Settings/FLChapter.h"
#include "LifeDev/Core/Settings/LFeatsMan.h"
#include "LifeDev/Core/Settings/LSave.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Core/Story/LStep.h"
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
	Settings->OnSaveReady.RemoveAll(this);

	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	
	ULGameInstance* const Instance = Cast<ULGameInstance>(GetGameInstance());
	if (!IsValid(Instance)){
		UE_LOG(LogLGameMode, Warning, TEXT("Game Mode: No valid instance found"));
		return;
	}
	
	ULSysSettings* const SysSettings = ULSysSettings::Get();
	if (!IsValid(SysSettings)) {
		UE_LOG(LogLGameMode, Warning, TEXT("System Settings not valid. can't continue."));
		return;
	}

	/// set flags
	UCAnimator::Debug = Settings->GetFeat(EFeat::DBG_ANIMS);
	UFlashback::Debug = Settings->GetFeat(EFeat::DBG_FB);
	AStep::Debug = Settings->GetFeat(EFeat::DBG_STEPS);
	UCSignificance::Debug = Settings->GetFeat(EFeat::DBG_SIG);

	/// post process (does this even works?)
	PostProcess = Cast<APostProcessVolume>(
		UGameplayStatics::GetActorOfClass(World, APostProcessVolume::StaticClass()));

	/// set input mode
	// this is critical or the dialogs will break
	APlayerController* const Controller = UGameplayStatics::GetPlayerController(World, 0);
	// these are not needed since we are using the input actions
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);
	Controller->bShowMouseCursor = false;

	/// Character
	Char = Cast<ALChar>(UGameplayStatics::GetActorOfClass(World, ALChar::StaticClass()));
	if (IsValid(Char)) {
		Char->InputPrio = 1;
		// Char->Init();
	} else {
		Char = nullptr;
	}

	/// music
	MusicMan = Cast<ALMusicMan>(World->SpawnActor(ALMusicMan::StaticClass()));

	/// flashback
	FlashbackMan = Cast<AFlashbackMan>(World->SpawnActor(AFlashbackMan::StaticClass()));
	
	/// Dialogs
	Diags = World->GetSubsystem<UDiags>();
	Diags->Init();

	DiagManager = Cast<ALDialogMan>(World->SpawnActor(ALDialogMan::StaticClass()));
	if (IsValid(DiagManager)) {
		// Needs to be 10 so that it takes precedence over the character
		DiagManager->InputPrio = 10;
		DiagManager->ZOrder = 3; 
		DiagManager->DebugSkip = !Settings->GetFeat(EFeat::D_ALL); // skip dialogs if no feature for it
		DiagManager->Init();
	} else {
		DiagManager = nullptr;
	}

	/// Inventory
	Flags = World->GetSubsystem<UFlags>();
	Flags->Init();
	Inventory = World->GetSubsystem<UInventory>();
	Inventory->Init(SysSettings->Inventory.LoadSynchronous());

	InvManager = Cast<ALInventoryManager>(World->SpawnActor(ALInventoryManager::StaticClass()));
	// InvManager = Cast<AInventoryManager>(UGameplayStatics::GetActorOfClass(World, AInventoryManager::StaticClass()));
	if (IsValid(InvManager)) {
		// goes below the dialogs. because some items will trigger a dialog.
		InvManager->InputPrio = 9;
		InvManager->ZOrder = 1; 
		InvManager->Init();
	} else {
		InvManager = nullptr;
	}

	/// Story
	Story = World->GetSubsystem<UStory>();
	Story->FadeTime = UJMiscUtils::IsEditor() ? 1: FadeTime;
	Story->HoldTime = UJMiscUtils::IsEditor() ? 1:  HoldTime;
	Story->Init();
	
	StoryManager = Cast<AStoryManager>(World->SpawnActor(AStoryManager::StaticClass()));
	// StoryManager = Cast<AStoryManager>(UGameplayStatics::GetActorOfClass(World, AStoryManager::StaticClass()));
	if (IsValid(StoryManager)) {
		StoryManager->ZOrder = 5;
		StoryManager->Init();
	} else {
		StoryManager = nullptr;
	}

	/// feats
	// do at the end since it depends on other things.
	FeatsMan = Cast<ALFeatsMan>(World->SpawnActor(ALFeatsMan::StaticClass()));
	
	///~ Subs-init finished.

	/// GameMode init starts
	// ensure the save-game loads the data into the subsystems.
	// do only after subsystems have been initialized.
	// do before StartChapter since that saves the gamefile (loading from subsystems)
	Settings->Save->WriteSubsystems(World);

	// start listening only here. in case the previous init might trigger a false one
	Diags->OnShow.AddUniqueDynamic(this, &ALGGameMode::DiagShown);
	Diags->OnDone.AddUniqueDynamic(this, &ALGGameMode::DiagDone);
	Story->OnSeqStop.AddUniqueDynamic(this, &ALGGameMode::StartNextChapter);
	Story->OnFade.AddUniqueDynamic(this, &ALGGameMode::Fade);
	
	// force the input disabled. even though the story manager will makes this disable later.
	// in case something goes wrong.
	// disable input on next tick to avoid a crash otherwise....
	World->GetTimerManager().SetTimerForNextTick(this, &ALGGameMode::SetInputDisable);

	FTimerHandle Handle;
	// wait for loading. then start the story!
	World->GetTimerManager().SetTimer(Handle, this, &ALGGameMode::StartChapter, .1);
}

void ALGGameMode::BeginPlay() {
	Super::BeginPlay();
	
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	
	ULGameInstance* const Instance = Cast<ULGameInstance>(GetGameInstance());
	if (!IsValid(Instance)) {
		UE_LOG(LogLGameMode, Warning, TEXT("No valid instance found"));
		return;
	}
	
	Settings = Instance->GetSubsystem<ULSettings>();
	if (!IsValid(Settings)) {
		UE_LOG(LogLGameMode, Warning, TEXT("Settings not valid. can't continue."));
		return;
	}

	// load the save and init. note the init is blocked until the save is available since we really
	// need that before hand. and can't work reliably without it.
	if (!IsValid(Settings->Save)) {
		UE_LOG(LogLGameMode, Warning, TEXT("Savegame not valid. Attempt to load or create"));
		Settings->OnSaveReady.AddUniqueDynamic(this, &ALGGameMode::Init);
		Settings->Init(); // force load. if it's currently loading then it won't re-trigger
		return;
	}

	// manually go to init if it's already loaded.	
	Init();
}

void ALGGameMode::DeInit_Implementation() {
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	
	if (IsValid(Diags)) {
		Diags->OnShow.RemoveAll(this);
		Diags->OnDone.RemoveAll(this);
		Diags->DeInit();
	}
	Diags = nullptr;

	if (IsValid(Inventory)) {
		Inventory->DeInit();
	}
	Inventory = nullptr;
	
	if (IsValid(Flags)) {
		Flags->DeInit();
	}
	Flags = nullptr;
	
	if (IsValid(DiagManager)) {
		DiagManager->DeInit();
	}
	DiagManager = nullptr;

	if (IsValid(InvManager)) {
		InvManager->DeInit();
	}
	InvManager = nullptr;

	if (IsValid(StoryManager)) {
		StoryManager->DeInit();
	}
	StoryManager = nullptr;

	if (IsValid(Story)) {
		Story->OnSeqStop.RemoveAll(this);
		Story->OnFade.RemoveAll(this);
	}
	Story = nullptr;

	if (IsValid(Char)) {
		// Char->DeInit();
	}
	Char = nullptr;

	if (IsValid(MusicMan)) {
		MusicMan->Fade(false); // probably won't get a chance to fade since the game mode is ending.
	}
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
	if(IsValid(Char)) {
		Char->SetInputEnabled(Enabled);
	}
	if (IsValid(InvManager)) {
		InvManager->SetVisible(Enabled);
	}
}

ALGGameMode* ALGGameMode::Get() {
	// TODO this doesn't work properly on PIE, fix and use the version in JMiscUtils
	if (!GEngine) return nullptr;
	UWorld* const World = GEngine->GetWorld();
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
	ULGameInstance* const Instance = Cast<ULGameInstance>(GetGameInstance());
	if (!IsValid(Instance) || !IsValid(Story)) {
		// Should this be here?
		UE_LOG(LogLGameMode, Warning, TEXT("No game instance or story or story manager. Can't proceed."));
		return;
	}
	
	// skip chapter if past end, or not enabled
	if (ChapterId <0 || ChapterId >= LDConsts::Feats::ChapFeatN ||
		!Settings->GetFeat(LDConsts::Feats::ChapFeats[ChapterId])) {
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

void ALGGameMode::Fade(bool bIn, const FText& Text) {
	if (!bIn) {
		SetInputDisable();
		return;
	}

	// fading in requires a timer.
	FTimerHandle Handle2;
	const float Wait = (Story->FadeTime)+Story->HoldTime;
	FTimerManager& Time = GetWorld()->GetTimerManager();
	Time.SetTimer(Handle2, this, &ALGGameMode::SetInputEnable, Wait, false);
}
