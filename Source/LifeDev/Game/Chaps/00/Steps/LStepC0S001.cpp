// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S001.h"

#include "Interact/Interact.h"
#include "Kismet/GameplayStatics.h"
#include "LifeDev/Game/Chaps/All/Env/Ghosts.h"
#include "LifeDev/Game/Chaps/All/Env/SRain.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC0S001::ALStepC0S001():Super() {
	Name = FName("C0S1");
	DlgId = FName("Intro");
	InputEnabled = false;
	UseFadeTime = true;
	UseFade = true;
	FinishPostWait = false;
}

void ALStepC0S001::Start_Implementation() {
	Super::Start_Implementation();
	if (IsValid(FakeChar)) {
		FakeChar->SetActorHiddenInGame(false);
	}

	UWorld* const W = GetWorld();
	UFlashback* const Flashback = UFlashback::Get(W);
	if (Flashback) {
		Flashback->SetMax(1);
		Flashback->SetVal(1, 10);
	}

	ASRain::SetPlayingS(W, true);

	Ghosts = Cast<AGhosts>(W->SpawnActor(AGhosts::StaticClass()));
	if (IsValid(Ghosts)) {
		Ghosts->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Ghosts->SetActorRelativeLocation(GhostLocation);
		Ghosts->SetPlaying(true);
	}

	FTimerHandle H;
	W->GetTimerManager().SetTimer(H, this, &ALStepC0S001::TeleportPlayer, WaitTime/2.0);
}

void ALStepC0S001::Stop_Implementation() {
	Super::Stop_Implementation();
	if (IsValid(FakeChar)) {
		FakeChar->Destroy(); // FakeChar->SetActorHiddenInGame(true);
	}
	UWorld* const W = GetWorld();
	if (IsValid(Ghosts)) {
		Ghosts->SetPlaying(false);
		// Destroy them on the fade
		FTimerHandle H;
		W->GetTimerManager().SetTimer(H, Ghosts, &AGhosts::K2_DestroyActor, WaitTime/2.0);
		// not destroying here to allow ghosts to de-spawn naturally
	}

	ASRain::SetPlayingS(W, false);
	UFlashback::SetValS(W, 0);
}

void ALStepC0S001::BeginPlay() {
	Super::BeginPlay();
	AInteract* const FakeInter = Cast<AInteract>(FakeChar);
	if (FakeInter) {
		FakeInter->SetEnabled(false);
	}
}

void ALStepC0S001::TeleportPlayer() {
	if (!IsValid(PlayerPos)) {
		UE_LOG(LogTemp, Log, TEXT("Player pos not set on C0S001"));
		return;
	}

	AActor* const Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ALChar::StaticClass());
	ALChar* const Char = Cast<ALChar>(Actor);
	if (!Char) return;
	
	Char->TeleportTo(PlayerPos->GetActorLocation(), PlayerPos->GetActorRotation());
}
