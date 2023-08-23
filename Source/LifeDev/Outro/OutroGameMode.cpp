// Copyright Jeronimo Barraco-Marmol

#include "OutroGameMode.h"

#include "OutroMan.h"
#include "GameFramework/SpectatorPawn.h"
#include "LifeDev/Core/Sound/MusicMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "UObject/ConstructorHelpers.h"

AOutroGameMode::AOutroGameMode():Super() {
	SetActorTickEnabled(false);
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = ASpectatorPawn::StaticClass();
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CMusic (TEXT("/Game/LifeDev/Game/Sounds/Music/Music00/Music00_MS.Music00_MS"));
	Music = CMusic.Object;
}

void AOutroGameMode::BeginPlay() {
	Super::BeginPlay();

	Manager = Cast<AOutroMan>(GetWorld()->SpawnActor<AOutroMan>(AOutroMan::StaticClass()));

	UWorld* const World = GetWorld();
	MusicMan = Cast<AMusicMan>(World->SpawnActor(AMusicMan::StaticClass()));
	MusicMan->PlayMusic(Music);

	UFlashback* const FB = UFlashback::Get(World);
	if (FB) {
		FB->SetVal(1);
	}
}

void AOutroGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	if (IsValid(Manager)) {
		Manager->Destroy();
	}
	Manager = nullptr;

	if (IsValid(MusicMan)) {
		MusicMan->Destroy();
		
	}
	MusicMan = nullptr;
}

