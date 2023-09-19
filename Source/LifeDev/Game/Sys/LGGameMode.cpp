// Copyright Jeronimo Barraco-Marmol

#include "LGGameMode.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Consts/ConstSettings.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "Dialogs/DialogManager.h"
#include "Interact/CInteract.h"
#include "Inventory/Inventory.h"
#include "Interact/CInteractor.h"
#include "Interact/Animator/CAnimator.h"
#include "Inventory/Flags.h"
#include "Inventory/InventoryManager.h"
#include "Story/StoryManager.h"
#include "Story/Story.h"
#include "Sounds/MusicMan.h"

#include "LifeDev/Core/LGameInstance.h"
#include "LifeDev/Core/Settings/FLChapter.h"
#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Core/Story/LStep.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Char/LGPController.h"
#include "LifeDev/Game/Dialogs/LDialogMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Inventory/LInventoryManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogLGameMode, Log, Log);

ALGGameMode::ALGGameMode():Super() {
	Super::SetActorTickEnabled(false);
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<ALChar>
		CChar(TEXT("/Game/LifeDev/Game/Char/LChar_B"));
	DefaultPawnClass = CChar.Succeeded() ? CChar.Class.Get(): ALChar::StaticClass();

	PlayerControllerClass = ALGPController::StaticClass();
	
	// UCInteractor::SetCollisionChannel(InteractTraceChannel);
	UCInteractor::SetCollisionChannel(ECC_Visibility);
	// UCInteract::CollisionProfile = "BlockAllDynamic";
	UCInteract::SetCollisionProfile("Interact");
	// UCInteract::CollisionProfile = "Interact";
}

bool ALGGameMode::LoadChapter() {
	ULSysSettings* const Settings = ULSysSettings::Get();
	UDataTable* const DT_Chaps = Settings->Chapters.LoadSynchronous();
	if (!IsValid(DT_Chaps)) {
		return false;
	}
	
	const FName ChapName = *FString::FromInt(ChapterId); // todo find a betterest way
	FLChapter* const pChap = DT_Chaps->FindRow<FLChapter>(ChapName, TEXT(""));
	if (!pChap) {
		UE_LOG(LogLGameMode, Warning, TEXT("Can't get the chapter from datatable"));
		return false;
	}

	Chapter = *pChap; // Make a copy
	// set them on the dialog subsystem
	UDataTable* const Chars = Settings->Characters.LoadSynchronous();
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
	
	ULSysSettings* const Settings = ULSysSettings::Get();
	if (!IsValid(Settings)) {
		UE_LOG(LogLGameMode, Warning, TEXT("Settings not valid"));
		return;
	}

	/// set input mode
	// this is critical or the dialogs will break
	APlayerController* const Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// these are not needed since we are using the input actions
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, true);
	Controller->bShowMouseCursor = false;

	/// set flags
	UCAnimator::Debug = Instance->GetFeat(EFeat::DEBUG_ANIMATOR);
	UFlashback::Debug = Instance->GetFeat(EFeat::DEBUG);
	AMusicMan::Enabled = Instance->GetFeat(EFeat::MUSIC);

	// todo should come from savestate
	ChapterId = 0;

	// music
	MusicMan = Cast<ALMusicMan>(World->SpawnActor(ALMusicMan::StaticClass()));

	/// Character
	Char = Cast<ALChar>(UGameplayStatics::GetActorOfClass(World, ALChar::StaticClass()));
	if (IsValid(Char)) {
		Char->InputPrio = 1;
		// Char->Init();
	} else {
		Char = nullptr;
	}

	/// Dialogs
	Dialogs = World->GetSubsystem<UDialogs>();
	Dialogs->Init();

	DiagManager = Cast<ALDialogMan>(World->SpawnActor(ALDialogMan::StaticClass()));
	if (IsValid(DiagManager)) {
		// Needs to be 10 so that it takes precedence over the character
		DiagManager->InputPrio = 10;
		DiagManager->ZOrder = 3; 
		DiagManager->DebugSkip = !Instance->GetFeat(EFeat::DIALOGS); // skip dialogs if no feature for it
		DiagManager->Init();
	} else {
		DiagManager = nullptr;
	}

	/// Inventory
	Flags = World->GetSubsystem<UFlags>();
	Flags->Init();
	Inventory = World->GetSubsystem<UInventory>();
	Inventory->Init(Settings->Inventory.LoadSynchronous());

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
	auto disableInput = [this] { // TODO maybe make this a function and be done. 
		SetCharInputEnabled(false);
	};
	// disable input on next tick to avoid a crash otherwise....
	FTimerDelegate Delegate;
	Delegate.BindLambda(disableInput);
	World->GetTimerManager().SetTimerForNextTick(Delegate);

	// start's the story
	FTimerHandle Handle;
	// StartStory();
	// wait for loading
	World->GetTimerManager().SetTimer(Handle, this, &ALGGameMode::StartChapter, 2.0);
}

void ALGGameMode::BeginPlay() {
	Super::BeginPlay();

	// TODO have a function to create the objects and create them here (what was this?)
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
		MusicMan->Fade(false); // probably won't get a chance to fade sinc ethe game mode is ending.
	}
	MusicMan = nullptr;
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
	// TODO this doesnt work properly on PIE, fix and use the version in JMiscUtils
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
	UE_LOG(LogLGameMode, Log, TEXT("Attempting to start chapter id=%i"), ChapterId);
	ULGameInstance* Instance = Cast<ULGameInstance>(GetGameInstance());
	if (!IsValid(Instance) || !IsValid(Story) || !IsValid(StoryManager)) {
		// Should this be here?
		UE_LOG(LogLGameMode, Warning, TEXT("No game instance or story or story manager. Can't proceed."));
		return;
	}
	
	// skip chapter if not enabled
	if (ChapterId < LDConsts::Feats::ChapFeatN) {
		if (!Instance->GetFeat(LDConsts::Feats::ChapFeats[ChapterId])) {
			UE_LOG(LogLGameMode, Warning, TEXT("Skipping chapter. Not in game Feats. id=%i."), ChapterId);
			StartNextChapter();
			return;
		}
	}

	if (!LoadChapter()) {
		UE_LOG(LogLGameMode, Warning, TEXT("Chapter didn't load. Won't start any sequence."));
		return;
	}
		
	// disable input only after conditions are met. only temp input in case the story decides to disable the whole character.
	SetTempInputEnabled(false);

	FText DecoratedTitle = FText::FromString(TEXT("~ ") + Chapter.Title.ToString() + TEXT(" ~"));
	StoryManager->FadeIn(DecoratedTitle);

	FTimerManager& Time = GetWorld()->GetTimerManager();
	FTimerHandle Handle1;
	FTimerDelegate Delegate1;
	// TODO make a function of this
	Delegate1.BindLambda([this] {
		Story->StartSequence(Chapter.StorySeq);
		// assuming this will return null if not able to load. which will stop the previous music, which is ok
		USoundBase* const Snd = Chapter.Music.LoadSynchronous();
		MusicMan->PlayMusic(Snd, true);
	});
	Time.SetTimer(Handle1, Delegate1, TimeFadeIn, false);
	
	FTimerHandle Handle2;
	Time.SetTimer(Handle2, StoryManager, &AStoryManager::FadeOut, TimeFadeIn+TimeHold);

	FTimerHandle Handle3;
	FTimerDelegate Delegate3;
	Delegate3.BindLambda([this] {
		SetTempInputEnabled(true);
	});
	Time.SetTimer(Handle3, Delegate3, TimeFadeIn+TimeFadeOut+TimeHold, false);
}

void ALGGameMode::StartNextChapter() {
	// Chapter done. go to the next one.
	++ChapterId;
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
	ALStep::FadeTime = TimeFadeOut+TimeHold+1.0;
}
