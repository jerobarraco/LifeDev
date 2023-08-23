// Copyright Jeronimo Barraco-Marmol

#include "IntroGameMode.h"

#include "GameFramework/SpectatorPawn.h"
#include "UObject/ConstructorHelpers.h"

#include "IntroMan.h"
#include "LifeDev/Core/Sound/MusicMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"

AIntroGameMode::AIntroGameMode():Super() {
	SetActorTickEnabled(false);
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = ASpectatorPawn::StaticClass();
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CMusic (TEXT("/Game/LifeDev/Game/Sounds/Music/Music00/Music00_MS.Music00_MS"));
	Music = CMusic.Object;
}

void AIntroGameMode::BeginPlay() {
	Super::BeginPlay();
	UWorld* const World = GetWorld();
	Manager = Cast<AIntroMan>(World->SpawnActor(AIntroMan::StaticClass()));
	MusicMan = Cast<AMusicMan>(World->SpawnActor(AMusicMan::StaticClass()));
	MusicMan->PlayMusic(Music);

	UFlashback* const FB = UFlashback::Get(World);
	if (FB) {
		FB->SetVal(1);
	}
}

void AIntroGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (IsValid(Manager)) {
		Manager->Destroy();
	}

	Manager = nullptr;
}
