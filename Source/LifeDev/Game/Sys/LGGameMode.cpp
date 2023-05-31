// Copyright Jeronimo Barraco-Marmol

#include "LGGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "Dialogs/DialogManager.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryManager.h"
#include "LifeDev/Core/LGameInstance.h"
#include "LifeDev/Core/Settings/FLChapter.h"

#include "Story/StoryManager.h"
#include "Story/Story.h"

#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Game/Char/LCharacter.h"

ALGGameMode::ALGGameMode():Super() {
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	// DefaultPawnClass = PlayerPawnClassFinder.Class;

	SetActorTickEnabled(false);
	DefaultPawnClass = ALCharacter::StaticClass();
}

bool ALGGameMode::LoadChapter() {
	ULSysSettings* const Settings = ULSysSettings::Get();
	UDataTable* const DT_Chaps = Settings->Chapters.LoadSynchronous();
	if (!IsValid(DT_Chaps)) {
		return true;
	}
	
	const FName ChapName = *FString::FromInt(ChapterId); // todo find a betterest way
	FLChapter* const pChap = DT_Chaps->FindRow<FLChapter>(ChapName, TEXT(""));
	if (!pChap) {
		UE_LOG(LogTemp, Warning, TEXT("Cant get the chapter from datatable"));
		return true;
	}

	Chapter = *pChap;
	UDataTable* const Chars = Settings->Characters.LoadSynchronous();
	UDataTable* const Diags = Chapter.Dialogs.LoadSynchronous();
	UDataTable* const Seqs = Chapter.Sequences.LoadSynchronous();
	Dialogs->SetData(Diags, Chars, Seqs);
	return false;
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
	const bool HasChap0 = Instance->HasFeat(EFeat::CHAP_00);
	ChapterId = HasChap0 ? 0: 1;
	
	/// Character
	Char = Cast<ALCharacter>(UGameplayStatics::GetActorOfClass(World, ALCharacter::StaticClass()));
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


	/////~

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
	UStory* const Story = World->GetSubsystem<UStory>();
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

	LoadChapter();
	
	FTimerHandle Handle;
	World->GetTimerManager().SetTimer(Handle, this, &ALGGameMode::StartStory, 3);
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

void ALGGameMode::StartStory() const {
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	// Should this be here?
	UStory* const Story = World->GetSubsystem<UStory>();
	if (!IsValid(Story)) return;

	Story->StartSequence(Chapter.StorySeq);
}

void ALGGameMode::DiagShown(const FDialog& Diag) {
	SetTempInputEnabled(false);
}
void ALGGameMode::DiagDone() {
	SetTempInputEnabled(true);
}
