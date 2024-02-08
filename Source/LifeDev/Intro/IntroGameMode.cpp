// Copyright Jeronimo Barraco-Marmol

#include "IntroGameMode.h"

#include "GameFramework/SpectatorPawn.h"
#include "UObject/ConstructorHelpers.h"

#include "IntroMan.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"

AIntroGameMode::AIntroGameMode():Super() {
	Super::SetActorTickEnabled(false);
	// set default pawn class to our Blueprinted character
	DefaultPawnClass = ASpectatorPawn::StaticClass();
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CMusic (TEXT("/Game/LifeDev/Game/Env/Music/Music00/Music00_MS.Music00_MS"));
	Music = CMusic.Object;
}

void AIntroGameMode::BeginPlay() {
	Super::BeginPlay();
	UWorld* const World = GetWorld();

	Manager = Cast<AIntroMan>(World->SpawnActor(AIntroMan::StaticClass()));
	MusicMan = Cast<ALMusicMan>(World->SpawnActor(ALMusicMan::StaticClass()));
	MusicMan->PlayMusic(Music);

	UFlashback* const FB = UFlashback::Instance(World);
	if (FB) {
		FB->SetVal(1);
	}

	// very important NOT to save the save-game here.
	// since none of the subsystems are initialized and it will save garbage
}

void AIntroGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (IsValid(Manager)) {
		Manager->Destroy();
	}

	Manager = nullptr;
}
