// Copyright Jeronimo Barraco-Marmol

#include "OutroGameMode.h"

#include "GameFramework/SpectatorPawn.h"
#include "UObject/ConstructorHelpers.h"

#include "OutroMan.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"

AOutroGameMode::AOutroGameMode():Super() {
	Super::SetActorTickEnabled(false);
	DefaultPawnClass = ASpectatorPawn::StaticClass(); // no need for a pawn
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CMusic (TEXT("/Game/LifeDev/Game/Sounds/Music/Music03/Music03_MS.Music03_MS"));
	Music = CMusic.Object;
}

void AOutroGameMode::BeginPlay() {
	Super::BeginPlay();

	Manager = Cast<AOutroMan>(GetWorld()->SpawnActor<AOutroMan>(AOutroMan::StaticClass()));

	UWorld* const World = GetWorld();
	MusicMan = Cast<ALMusicMan>(World->SpawnActor(ALMusicMan::StaticClass()));
	MusicMan->PlayMusic(Music, true);

	UFlashback* const FB = UFlashback::Get(World);
	if (FB) {
		FB->SetVal(1, 30);
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

