// Copyright Jeronimo Barraco-Marmol

#include "IntroGameMode.h"

#include "GameFramework/SpectatorPawn.h"
#include "UObject/ConstructorHelpers.h"

AIntroGameMode::AIntroGameMode():Super() {
	SetActorTickEnabled(false);
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<ALChar>
		// CChar(TEXT("/Game/LifeDev/Game/Char/LChar_B"));
	DefaultPawnClass = ASpectatorPawn::StaticClass();
}
