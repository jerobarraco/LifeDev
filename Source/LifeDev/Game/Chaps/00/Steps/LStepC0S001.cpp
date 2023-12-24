// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S001.h"

#include "Camera/CameraComponent.h"
#include "Interact/Interact.h"
#include "Kismet/GameplayStatics.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Chaps/All/Env/Ghosts.h"

#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC0S001::ALStepC0S001():Super() {
	Name = FName("C0S1");
	DlgId = FName("Intro");
	InputEnabled = false;
	UseFadeTime = true;
	UseFade = true;
	FinishPostWait = false;
	TeleportChar = true;

	Cam->SetConstraintAspectRatio(true);
	Cam->SetAspectRatio(2);
}

void ALStepC0S001::Start_Implementation() {
	Super::Start_Implementation();
	if (IsValid(FakeChar)) {
		FakeChar->SetActorHiddenInGame(false);
		FakeChar->SetEnabled(true);
	}

	UWorld* const W = GetWorld();
	FB->SetMax(1);
	FB->SetVal(1, 10);

	ALMusicMan::SetRainS(W, true);

	Ghosts = Cast<AGhosts>(W->SpawnActor(AGhosts::StaticClass()));
	if (IsValid(Ghosts)) {
		Ghosts->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Ghosts->SetActorRelativeLocation(GhostLocation);
		Ghosts->SetPlaying(true);
	}
}

void ALStepC0S001::Stop_Implementation() {
	Super::Stop_Implementation();
	UWorld* const W = GetWorld();
	if (IsValid(Ghosts)) {
		Ghosts->SetPlaying(false);
	}
	// Destroy them during the fade
	// FTimerHandle H;
	// W->GetTimerManager().SetTimer(H, this, &ALStepC0S001::DestroyActors, WaitTime);
	DestroyActors(); // stop now gets called while it's fading. so wait time actually is wrong.

	ALMusicMan::SetRainS(W, false);
	FB->SetVal(0);
}

void ALStepC0S001::DestroyActors() {
	// this is a bit dangerous, we can't go back to chap 0 without reloading.
	// but also more performant.

	if (IsValid(FakeChar)) {
		FakeChar->Destroy(); // FakeChar->SetActorHiddenInGame(true);
	}
	FakeChar = nullptr;

	if (IsValid(Ghosts)) {
		Ghosts->Destroy();
	}
	Ghosts = nullptr;
}

void ALStepC0S001::BeginPlay() {
	Super::BeginPlay();
	if (FakeChar) {
		FakeChar->SetEnabled(false);
	}
}

void ALStepC0S001::TeleportPlayer() {
	// TODO use TeleportChar on the next step and use the step position in the world instead :V
	if (!IsValid(PlayerPos)) {
		UE_LOG(LogTemp, Log, TEXT("Player pos not set on C0S001"));
		return;
	}

	ALChar* const Char = Cast<ALChar>(UGameplayStatics::GetActorOfClass(GetWorld(), ALChar::StaticClass()));
	if (!Char) return;
	
	Char->TeleportTo(PlayerPos->GetActorLocation(), PlayerPos->GetActorRotation());
}
