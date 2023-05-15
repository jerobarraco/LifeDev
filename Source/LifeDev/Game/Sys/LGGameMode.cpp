// Copyright Jeronimo Barraco-Marmol

#include "LGGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#include "LifeDev/Game/Char/LCharacter.h"
#include "LifeDev/Game/Dialogs/DialogManager.h"

ALGGameMode::ALGGameMode():Super() {
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	// DefaultPawnClass = PlayerPawnClassFinder.Class;

	SetActorTickEnabled(false);
	DefaultPawnClass = ALCharacter::StaticClass();
}

void ALGGameMode::Init() const {
	// this is the place were we are going to be initializing everything. -Jero
	ADialogManager* const DiagManager = Cast<ADialogManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ADialogManager::StaticClass()));
	if (IsValid(DiagManager)) {
		DiagManager->Init();
	}
}

void ALGGameMode::BeginPlay() {
	Super::BeginPlay();

	Init();
}
