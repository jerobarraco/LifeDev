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
		CMusic (TEXT("/Game/LifeDev/Game/Env/Music/Music03/Music03_MS.Music03_MS"));
	Music = CMusic.Object;
}

void AOutroGameMode::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	
	Manager = Cast<AOutroMan>(World->SpawnActor<AOutroMan>(AOutroMan::StaticClass()));
	MusicMan = Cast<ALMusicMan>(World->SpawnActor(ALMusicMan::StaticClass()));

	UFlashback* const FB = UFlashback::Instance(World);
	if (LIKELY(FB)) {
		FB->Init();
		FB->SetVal(1, 5);
	}
	
	if (LIKELY(MusicMan)) {
		MusicMan->Init(); // very important
		MusicMan->Play(Music, false);
	}
}

void AOutroGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	if (LIKELY(IsValid(Manager)))
		Manager->Destroy();
	Manager = nullptr;

	if (LIKELY(IsValid(MusicMan)))
		MusicMan->Destroy();
	MusicMan = nullptr;
}

