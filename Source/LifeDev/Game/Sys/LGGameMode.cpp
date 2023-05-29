// Copyright Jeronimo Barraco-Marmol

#include "LGGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "Dialogs/DialogManager.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryManager.h"

#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Game/Char/LCharacter.h"
#include "Story/StoryManager.h"

ALGGameMode::ALGGameMode():Super() {
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	// DefaultPawnClass = PlayerPawnClassFinder.Class;

	SetActorTickEnabled(false);
	DefaultPawnClass = ALCharacter::StaticClass();
}

void ALGGameMode::Init_Implementation() {
	// this is the place were we are going to be initializing everything.

	UWorld* const World = GetWorld();

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

	ULSysSettings* const Settings = ULSysSettings::Get();
	UDialogs* const Dialogs = World->GetSubsystem<UDialogs>();
	if (IsValid(Settings) && IsValid(Dialogs) && Chapter < Settings->ChapDialogs.Num() && Chapter<Settings->Sequences.Num())  {
		UDataTable* const DT = Settings->ChapDialogs[Chapter].LoadSynchronous();
		UDataTable* const Chars = Settings->Characters.LoadSynchronous();
		UDataTable* const Seqs = Settings->Sequences[Chapter].LoadSynchronous();
		Dialogs->Init(DT, Chars, Seqs);
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Did not loaded the dialog tables for some weird reason"));
	}

	/// Inventory
	UInventory* const Inventory =  World->GetSubsystem<UInventory>();
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
	StoryManager = Cast<AStoryManager>(UGameplayStatics::GetActorOfClass(World, AStoryManager::StaticClass()));
	if (IsValid(StoryManager)) {
		StoryManager->Init();
	} else {
		StoryManager = nullptr;
	}

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
	UDialogs* const Dialogs = World->GetSubsystem<UDialogs>();
	if (IsValid(Dialogs)) {
		Dialogs->DeInit();
	}
	
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
	if(!IsValid(Char)) return;
	Char->SetInputEnabled(Enabled);
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
	if (!IsValid(StoryManager)) return;
	StoryManager->Start(FName("C0S0")); // TODO put these names somewhere else
}
