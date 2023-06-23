// Copyright Jeronimo Barraco-Marmol

#include "LGGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "Dialogs/DialogManager.h"
#include "Interact/CInteract.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryManager.h"
#include "LifeDev/Core/LGameInstance.h"
#include "LifeDev/Core/Settings/FLChapter.h"

#include "Story/StoryManager.h"
#include "Story/Story.h"
#include "Interact/CInteractor.h"

#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Core/Story/LStep.h"
#include "LifeDev/Game/Pawn/LChar.h"
#include "LifeDev/Game/Pawn/LGPController.h"

ALGGameMode::ALGGameMode():Super() {
	SetActorTickEnabled(false);
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
		UE_LOG(LogTemp, Warning, TEXT("Can't get the chapter from datatable"));
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
	if (!IsValid(Instance)) return;
	
	ULSysSettings* const Settings = ULSysSettings::Get();
	if (!IsValid(Settings)) {
		UE_LOG(LogTemp, Warning, TEXT("Settings not valid"));
		return;
	}

	// todo improve. should come from savestate
	const bool HasChap0 = Instance->GetFeat(EFeat::CHAP_00);
	ChapterId = HasChap0 ? 0: 1;
	
	/// Character
	Char = Cast<ALChar>(UGameplayStatics::GetActorOfClass(World, ALChar::StaticClass()));
	if (IsValid(Char)) {
		Char->InputPrio = 1; // todo move this inside init
		// Char->Init();
	} else {
		Char = nullptr;
	}
	
	/// Dialogs
	DiagManager = Cast<ADialogManager>(UGameplayStatics::GetActorOfClass(World, ADialogManager::StaticClass()));
	if (IsValid(DiagManager)) {
		// Needs to be 10 so that it takes precedence over the character
		DiagManager->InputPrio = 10; // todo pass inside init
		DiagManager->Init(); 
	} else {
		DiagManager = nullptr;
	}

	/// Inventory
	UInventory* const Inventory = World->GetSubsystem<UInventory>();
	Inventory->Init(Settings->Inventory.LoadSynchronous());

	InvManager = Cast<AInventoryManager>(UGameplayStatics::GetActorOfClass(World, AInventoryManager::StaticClass()));
	if (IsValid(InvManager)) {
		// goes below the dialogs. because some items will trigger a dialog.
		InvManager->InputPrio = 9;
		InvManager->Init();
	} else {
		InvManager = nullptr;
	}

	/// Story
	Story = World->GetSubsystem<UStory>();
	Story->OnSeqStop.AddUniqueDynamic(this, &ALGGameMode::StartNextChapter);
	Story->Init();

	StoryManager = Cast<AStoryManager>(UGameplayStatics::GetActorOfClass(World, AStoryManager::StaticClass()));
	if (IsValid(StoryManager)) {
		StoryManager->Init();
	} else {
		StoryManager = nullptr;
	}

	Dialogs = World->GetSubsystem<UDialogs>();
	Dialogs->Init();
	Dialogs->OnShow.AddUniqueDynamic(this, &ALGGameMode::DiagShown);
	Dialogs->OnDone.AddUniqueDynamic(this, &ALGGameMode::DiagDone);

	// start by disabling the input
	auto disableInput = [this] {
		SetCharInputEnabled(false);
	};
	// disable input on next tick to avoid a crash otherwise....
	FTimerDelegate Delegate;
	Delegate.BindLambda(disableInput);
	World->GetTimerManager().SetTimerForNextTick(Delegate);

	// start's the story
	// FTimerHandle Handle;
	// StartStory();
	// World->GetTimerManager().SetTimer(Handle, this, &ALGGameMode::StartStory, 1.0);
	World->GetTimerManager().SetTimerForNextTick(this, &ALGGameMode::StartStory);
}

void ALGGameMode::BeginPlay() {
	Super::BeginPlay();

	Init();
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

	UInventory* const Inventory = World->GetSubsystem<UInventory>();
	if (IsValid(Inventory)) {
		Inventory->DeInit();
	}
	
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

void ALGGameMode::StartStory() {
	// Should this be here?
	if (!IsValid(Story)) return;
	if (!LoadChapter()) {
		UE_LOG(LogTemp, Warning, TEXT("Chapter didn't load. Won't start any sequence."));
		return;
	}

	// TODO make these times into parameters
	
	// disable input only after conditions are met. only temp input in case the story decides to disable the whole character.
	SetTempInputEnabled(false);

	FText DecoratedTitle = FText::FromString(TEXT("~ ") + Chapter.Title.ToString() + TEXT(" ~"));
	StoryManager->FadeIn(DecoratedTitle);
	
	FTimerManager& Time = GetWorld()->GetTimerManager();
	FTimerHandle Handle1;
	FTimerDelegate Delegate1;
	Delegate1.BindLambda([this] {
		Story->StartSequence(Chapter.StorySeq);
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
	StartStory();
}

void ALGGameMode::DiagShown(const FDialog& Diag) {
	SetTempInputEnabled(false);
}

void ALGGameMode::DiagDone() {
	SetTempInputEnabled(true);
}

void ALGGameMode::PostLoad() {
	Super::PostLoad();
	ALStep::FadeTime = TimeFadeOut+TimeHold;
}
