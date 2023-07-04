// Copyright Jeronimo Barraco-Marmol

#include "IntroGameMode.h"

#include "GameFramework/SpectatorPawn.h"
#include "UObject/ConstructorHelpers.h"

#include "IntroMan.h"

AIntroGameMode::AIntroGameMode():Super() {
	SetActorTickEnabled(false);
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = ASpectatorPawn::StaticClass();
}

void AIntroGameMode::BeginPlay() {
	Super::BeginPlay();
	Manager = Cast<AIntroMan>(GetWorld()->SpawnActor(AIntroMan::StaticClass()));
}

void AIntroGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (IsValid(Manager)) {
		Manager->Destroy();
	}

	Manager = nullptr;
}
