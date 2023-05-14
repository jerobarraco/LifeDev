// Copyright Jeronimo Barraco-Marmol

#include "LGMGame.h"

#include "UObject/ConstructorHelpers.h"

#include "LifeDev/Game/Char/LCharacter.h"

ALGMGame::ALGMGame():Super() {
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	// DefaultPawnClass = PlayerPawnClassFinder.Class;

	DefaultPawnClass = ALCharacter::StaticClass();
}

void ALGMGame::BeginPlay() {
	Super::BeginPlay();
}
