// Copyright Jeronimo Barraco-Marmol

#include "OutroGameMode.h"

#include "GameFramework/SpectatorPawn.h"
#include "UObject/ConstructorHelpers.h"

#include "Inventory/Flags.h"

#include "LifeDev/Core/Settings/LSave.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "OutroMan.h"

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

	/// load data
	// i won't bother checking for the savestate and loading the data as it should be here already.
	const ULSettings* const Settings = ULSettings::Instance(this);
	if (Settings && Settings->Save)
		Settings->Save->WriteSubsystems(this);
	
	/// spawn
	Manager = Cast<AOutroMan>(World->SpawnActor<AOutroMan>(AOutroMan::StaticClass()));
	MusicMan = Cast<ALMusicMan>(World->SpawnActor(ALMusicMan::StaticClass()));

	/// init stuff
	/// subsystems
	UFlashback* const FB = UFlashback::Instance(World);
	if (LIKELY(FB)) {
		FB->Init();
		FB->SetVal(1, 5);
	}

	UFlags* const Flags = UFlags::Instance(this);
	if (LIKELY(Flags)) Flags->Init();

	/// managers
	if (LIKELY(MusicMan)) {
		MusicMan->Init(); // very important
		MusicMan->Play(Music, false);
	}

	if (LIKELY(Manager)) Manager->Init();
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

