// Copyright Jeronimo Barraco-Marmol

#include "LGameMode.h"

#include "LifeDev/Core/Char/LCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALGameMode::ALGameMode() : Super() {
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	// DefaultPawnClass = PlayerPawnClassFinder.Class;

	DefaultPawnClass = ALCharacter::StaticClass();
}
