// Copyright Jeronimo Barraco-Marmol

#include "LGGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "Dialogs/DialogManager.h"

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
	DiagManager = Cast<ADialogManager>(UGameplayStatics::GetActorOfClass(World, ADialogManager::StaticClass()));
	if (IsValid(DiagManager)) {
		DiagManager->Init();
	} else {
		DiagManager = nullptr;
	}

	ULSysSettings* const Settings = ULSysSettings::Get();
	UDialogs* const Dialogs = World->GetSubsystem<UDialogs>();
	constexpr uint8 ChapId = 1;
	if (IsValid(Settings) && IsValid(Dialogs) && ChapId < Settings->ChapDialogs.Num())  {
		UDataTable* const DT = Settings->ChapDialogs[ChapId].LoadSynchronous();
		Dialogs->Load(DT);
	}
}

void ALGGameMode::BeginPlay() {
	Super::BeginPlay();

	Init();
}
