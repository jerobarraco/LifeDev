// Copyright Jeronimo Barraco-Marmol

#include "IntroGameMode.h"

#include "GameFramework/SpectatorPawn.h"
#include "UObject/ConstructorHelpers.h"

#include "IntroMan.h"
#include "SentrySubsystem.h"
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
	}

	UFlashback* const FB = UFlashback::Instance(World);
	if (LIKELY(FB)) {
		FB->Init();
		FB->SetVal(1, .15);
	}

	if (LIKELY(MusicMan)) MusicMan->Init(); // very important

	// very important NOT to save the save-game here.
	// since none of the subsystems are initialized and it will save garbage

	const USentrySubsystem* const Sentry = GEngine->GetEngineSubsystem<USentrySubsystem>();
	// if (SentrySubsystem)
		// SentrySubsystem->CaptureMessage(TEXT("Capture message"));
	UE_CLOG(!Sentry, LogTemp, Error, TEXT("Sentry does not work!"));
	// Sentry->IsCrashedLastRun() // interesting
	// Sentry->IsEnabled() // interesting
}

void AIntroGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(IsValid(Manager))) Manager->Destroy();
	Manager = nullptr;
	if (LIKELY(IsValid(MusicMan))) MusicMan->Destroy();
	MusicMan = nullptr;

	Super::EndPlay(EndPlayReason);
}
