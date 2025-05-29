// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "Puzzle.h"

#include "Interact.h"

#include "Interact/CPuzzle.h"

APuzzle::APuzzle():Super() {
	CPuzzle = CreateDefaultSubobject<UCPuzzle>(TEXT("CPuzzle"));
	CPuzzle->DisableOnDone = true;
	UseAnim = false;
	UseAutoActivate = false; // by default i'm not using this puzzle as directly.
}

void APuzzle::Reset() {
	if (LIKELY(CPuzzle)) CPuzzle->Reset();

	ClearTimer();
}

void APuzzle::Done_Implementation(const bool IsOk) {
	UE_LOG(LogTemp, Log, TEXT("APuzzle::Done ok=%i o=%s"), IsOk, *GetNameSafe(this));
	if (!IsOk) {
		// reset if needed. but not inside done. Since done is overrideable and can change orders
		// it will mess with the logical flow anyway.
		// this is important to be done on the Puzzle since Done is overrideable and hence can be postponed if needed
		if (ResetOnFail) {
			ClearTimer();
			const UWorld* const W = GetWorld();
			if (LIKELY(W)) W->GetTimerManager().SetTimerForNextTick(this, &APuzzle::Reset);
		}
		return;
	}

	if (IsValid(DoneInter)) {
		DoneInter->IsLocked = false; // force unlock
		DoneInter->TryTrigger();
	}

	if (IsValid(DoneActor)) {
		DoneActor->SetActorHiddenInGame(false);
		AInteract* const Reward = Cast<AInteract>(DoneActor);
		if (IsValid(Reward)) Reward->SetActive(true);
	}
}

void APuzzle::Update_Implementation() {
	// note update is called before done. so it's safe to re add the timer. done will clear it if needed.
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;
	
	ClearTimer();
	// re-add the reset timer if needed. Notice all the types return when done
	if (ResetTimeout >= 0)
		W->GetTimerManager().SetTimer(ResetTimer, this, &APuzzle::Reset, ResetTimeout);
}

void APuzzle::BeginPlay() {
	Super::BeginPlay();

	if (LIKELY(IsValid(CPuzzle))) {
		CPuzzle->OnDone.AddUniqueDynamic(this, &APuzzle::Done);
		CPuzzle->OnUpdate.AddUniqueDynamic(this, &APuzzle::Update);
		CPuzzle->OnReset.AddUniqueDynamic(this, &APuzzle::DoReset);
	} else {
		UE_LOG(LogTemp, Warning,
			TEXT("CPuzzle in Puzzle is not valid!! Huge problem. o=%s"),
			*GetNameSafe(this));
	}
	
	if (IsValid(DoneActor)) {
		DoneActor->SetActorHiddenInGame(true);
		AInteract* const Reward = Cast<AInteract>(DoneActor);
		if (IsValid(Reward)) Reward->SetActive(false);
	}
}

void APuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(CPuzzle)) {
		CPuzzle->OnDone.RemoveAll(this);
		CPuzzle->OnUpdate.RemoveAll(this);
		CPuzzle->OnReset.RemoveAll(this);
	}
	CPuzzle = nullptr;

	ClearTimer();

	Super::EndPlay(EndPlayReason);
}

void APuzzle::ClearTimer() {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;
	
	W->GetTimerManager().ClearTimer(ResetTimer);
	ResetTimer.Invalidate();
}
