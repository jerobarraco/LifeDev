// Copyright Jeronimo Barraco-Marmol

#include "OutroGameMode.h"

#include "GameFramework/SpectatorPawn.h"
#include "UObject/ConstructorHelpers.h"

AOutroGameMode::AOutroGameMode():Super() {
	SetActorTickEnabled(false);
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = ASpectatorPawn::StaticClass();
}
