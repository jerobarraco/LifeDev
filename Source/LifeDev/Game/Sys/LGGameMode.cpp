// Copyright Jeronimo Barraco-Marmol

#include "LGGameMode.h"

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

#include "LifeDev/Core/LGameInstance.h"
#include "LifeDev/Core/Settings/FLChapter.h"
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
	UDataTable* const Diags = Chapter.Dialogs.LoadSynchronous();
	UDataTable* const Seqs = Chapter.Sequences.LoadSynchronous();
	Dialogs->SetData(Diags, Chars, Seqs);
	return true;
}

void ALGGameMode::Init_Implementation() {
	// this is the place were we are going to be initializing everything.

	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	
	ULGameInstance* const Instance = Cast<ULGameInstance>(GetGameInstance());
	if (!IsValid(Instance)){
		UE_LOG(LogLGameMode, Warning, TEXT("Game Mode: No valid instance found"));
		return;
	}
	
	ULSysSettings* const SysSettings = ULSysSettings::Get();
	if (!IsValid(SysSettings)) {
		UE_LOG(LogLGameMode, Warning, TEXT("System Settings not valid. cant continue."));
		return;
	}

	// allow quickmesh to skip wpo
	World->Exec(World, TEXT("r.OptimizedWPO 1"));
	
	//TODO implement save/load from ui. for now we always start a new one.
	// that will have to happen on the intro level on another game mode
	Settings = Instance->GetSubsystem<ULSettings>();
	if (!IsValid(Settings)) {
		UE_LOG(LogLGameMode, Warning, TEXT("Settings not valid. cant continue."));
		return;
	}
	
	Settings->NewGame();

	/// set flags
	UCAnimator::Debug = Settings->GetFeat(EFeat::DBG_ANIMS);
	UFlashback::Debug = Settings->GetFeat(EFeat::DBG_FB);
	AStep::UseDebug = Settings->GetFeat(EFeat::DBG_STEPS);

	/// post process (does this even works?)
	PostProcess = Cast<APostProcessVolume>(
		UGameplayStatics::GetActorOfClass(World, APostProcessVolume::StaticClass()));

	if (IsValid(PostProcess)) {
		// Important:
		// these properties on the editor have a checkbox next to them.
		// i DO need to check them for the engine to pay attention to them,
		// otherwise the changes here make no difference.
		// https://forums.unrealengine.com/t/how-can-i-control-post-processing-volume-settings-using-c/465187/2?u=nande
		const bool HasLumen = Settings->GetFeat(EFeat::V_LUMEN);
		PostProcess->Settings.DynamicGlobalIlluminationMethod =
			HasLumen ?
			EDynamicGlobalIlluminationMethod::Lumen : EDynamicGlobalIlluminationMethod::None;
		PostProcess->Settings.ReflectionMethod =
			HasLumen ? EReflectionMethod::Lumen : EReflectionMethod::None;
		if (!Settings->GetFeat(EFeat::V_BLUR)) {
			PostProcess->Settings.MotionBlurAmount = 0;
			PostProcess->Settings.MotionBlurMax = 0;
			PostProcess->Settings.SceneFringeIntensity = 0;
		}
	}

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
	Dialogs = World->GetSubsystem<UDiags>();
	Dialogs->Init();

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
	Story->FadeTime = FadeTime;
	Story->HoldTime = HoldTime;
	Story->Init();
	
	StoryManager = Cast<AStoryManager>(World->SpawnActor(AStoryManager::StaticClass()));
	// StoryManager = Cast<AStoryManager>(UGameplayStatics::GetActorOfClass(World, AStoryManager::StaticClass()));
	if (IsValid(StoryManager)) {
		StoryManager->ZOrder = 5;
		StoryManager->Init();
	} else {
		StoryManager = nullptr;
	}

	/// others' init finalized, finish my init
	// start listening only here. in case the previous init might trigger a false one
	Dialogs->OnShow.AddUniqueDynamic(this, &ALGGameMode::DiagShown);
	Dialogs->OnDone.AddUniqueDynamic(this, &ALGGameMode::DiagDone);
	Story->OnSeqStop.AddUniqueDynamic(this, &ALGGameMode::StartNextChapter);

	// start by disabling the input
	auto disableInput = [this] { // TODO promote to function, i bind this several times
		SetCharInputEnabled(false);
	};
	// disable input on next tick to avoid a crash otherwise....
	FTimerDelegate Delegate;
	Delegate.BindLambda(disableInput);
	World->GetTimerManager().SetTimerForNextTick(Delegate);

	// start's the story
	FTimerHandle Handle;
	// wait for loading
	World->GetTimerManager().SetTimer(Handle, this, &ALGGameMode::StartChapter, 2.0);
	MusicMan->SetEnviron(true);
}

void ALGGameMode::BeginPlay() {
	Super::BeginPlay();

	Init();
	// SetDynRes(); // can break the game and it's unused anyway
}

void ALGGameMode::DeInit_Implementation() {
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	
	if (IsValid(Dialogs)) {
		Dialogs->OnShow.RemoveAll(this);
		Dialogs->OnDone.RemoveAll(this);
		Dialogs->DeInit();
	}
	Dialogs = nullptr;

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
	CharInputEnabled = Enabled;
	SetTempInputEnabled(Enabled);
}

void ALGGameMode::SetTempInputEnabled(bool Enabled) {
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

void ALGGameMode::SetDynRes() {
	// Note this is not even being called. but left in case
	
	// https://docs.unrealengine.com/5.2/en-US/dynamic-resolution-in-unreal-engine/
	if (!UseDynRes || !GEngine) return;
	GEngine->SetDynamicResolutionUserSetting(true);
	// GEngine->GameUserSettings->ApplyNonResolutionSettings();
	GetWorld()->Exec(GetWorld(), TEXT("r.DynamicRes.OperationMode 2"));
	GetWorld()->Exec(GetWorld(), TEXT("r.DynamicRes.MinScreenPercentage 50"));
	GetWorld()->Exec(GetWorld(), TEXT("r.DynamicRes.MaxScreenPercentage 100"));
	GetWorld()->Exec(GetWorld(), TEXT("r.DynamicRes.FrameTimeBudget 33.33333"));
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
		StartNextChapter();
		return;
	}

	if (!LoadChapter()) {
		UE_LOG(LogLGameMode, Warning, TEXT("Chapter didn't load. Won't start any sequence."));
		return;
	}

	Settings->SaveGame();

	// disable input only after conditions are met. only temp input in case the story decides to disable the whole character.
	SetTempInputEnabled(false);
	
	Story->StartSequence(Chapter.Steps);

	FTimerHandle Handle2;
	FTimerDelegate Delegate2;
	Delegate2.BindLambda([this] {
		SetTempInputEnabled(true);
	});
	FTimerManager& Time = GetWorld()->GetTimerManager();
	Time.SetTimer(Handle2, Delegate2, (Story->FadeTime*2)+Story->HoldTime, false);
}

void ALGGameMode::StartNextChapter() {
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

void ALGGameMode::PostLoad() {
	Super::PostLoad();
	// TODO this might not be necessary. it shouldn't
	ALStep::FadeTime = (FadeTime*2)+ HoldTime;
}
