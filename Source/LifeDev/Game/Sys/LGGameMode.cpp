// Copyright Jeronimo Barraco-Marmol

#include "LGGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "Dialogs/DialogManager.h"
#include "Inventory/Inventory.h"

#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Game/Char/LCharacter.h"

ALGGameMode::ALGGameMode():Super() {
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	// DefaultPawnClass = PlayerPawnClassFinder.Class;

	SetActorTickEnabled(false);
	DefaultPawnClass = ALCharacter::StaticClass();
}

void ALGGameMode::Init() {
	// this is the place were we are going to be initializing everything. -Jero

	UWorld* const World = GetWorld();
	/// Dialogs
	DiagManager = Cast<ADialogManager>(UGameplayStatics::GetActorOfClass(World, ADialogManager::StaticClass()));
	if (IsValid(DiagManager)) {
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
	}


	/// Inventory
	UInventory* const Inventory =  World->GetSubsystem<UInventory>();
	Inventory->Init(nullptr);
}

void ALGGameMode::BeginPlay() {
	Super::BeginPlay();

	Init();
}

void ALGGameMode::DeInit() {
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	UDialogs* const Dialogs = World->GetSubsystem<UDialogs>();
	if (!IsValid(Dialogs)) return;
	Dialogs->DeInit();
	UInventory* const Inventory = World->GetSubsystem<UInventory>();
	if (!IsValid(Inventory)) return;
	Inventory->DeInit();

	DiagManager = nullptr;
}

void ALGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DeInit();
	Super::EndPlay(EndPlayReason);
}
