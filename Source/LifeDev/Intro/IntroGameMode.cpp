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
}

void AIntroGameMode::BeginPlay() {
	Super::BeginPlay();
	UWorld* const World = GetWorld();

	Manager = Cast<AIntroMan>(World->SpawnActor(AIntroMan::StaticClass()));
	MusicMan = Cast<ALMusicMan>(World->SpawnActor(ALMusicMan::StaticClass()));
	if (LIKELY(MusicMan)) {
		// fix so that the environment doesn't play during the intro level
		MusicMan->SetEnvironOverride(false);
		MusicMan->SetGhostOverride(false);
		// needed because musicman "turns on", on begin play. which already happened after spawn.
		MusicMan->SetGhosts(false);
	}

	UFlashback* const FB = UFlashback::Instance(World);
	if (LIKELY(FB)) FB->SetVal(1);

	// very important NOT to save the save-game here.
	// since none of the subsystems are initialized and it will save garbage
}

void AIntroGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(IsValid(Manager))) Manager->Destroy();
	Manager = nullptr;
	if (LIKELY(IsValid(MusicMan))) MusicMan->Destroy();
	MusicMan = nullptr;

	Super::EndPlay(EndPlayReason);
}
