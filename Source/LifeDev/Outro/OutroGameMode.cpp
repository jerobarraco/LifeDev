// Copyright Jeronimo Barraco-Marmol

#include "OutroGameMode.h"

#include "OutroMan.h"
#include "GameFramework/SpectatorPawn.h"
#include "UObject/ConstructorHelpers.h"

AOutroGameMode::AOutroGameMode():Super() {
	SetActorTickEnabled(false);
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = ASpectatorPawn::StaticClass();
}

void AOutroGameMode::BeginPlay() {
	Super::BeginPlay();
	Manager = Cast<AOutroMan>(GetWorld()->SpawnActor<AOutroMan>(AOutroMan::StaticClass()));
}

void AOutroGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (IsValid(Manager)) {
		Manager->Destroy();
	}

	Manager = nullptr;
}

