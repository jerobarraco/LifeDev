// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "Puzzle.h"

#include "Interact.h"
#include "Components/BillboardComponent.h"
#include "Interact/CPuzzle.h"

APuzzle::APuzzle():Super() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	CPuzzle = CreateDefaultSubobject<UCPuzzle>(TEXT("CPuzzle"));
	CPuzzle->DisableOnDone = true;

#if WITH_EDITORONLY_DATA // inspired by AInfo
	UBillboardComponent* const SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	if (LIKELY(!IsRunningCommandlet() && (SpriteComponent != nullptr))) {
		// static ConstructorHelpers::FObjectFinderOptional<UTexture2D> SpriteTexture(TEXT("/Engine/EditorResources/S_Actor"));
		// static ConstructorHelpers::FObjectFinder<UTexture2D> CSprText(TEXT("/Engine/EditorResources/S_Actor"));
		static ConstructorHelpers::FObjectFinderOptional<UTexture2D>
			CSprTexture(TEXT("/Engine/EditorResources/S_Actor")); // S_Solver
		static const FName ID_Info = TEXT("Puzzle");
		static const FText ID_Name = NSLOCTEXT("SpriteCategory", "Puzzle", "Puzzle");

		SpriteComponent->SetupAttachment(Root);
		SpriteComponent->Sprite = CSprTexture.Get();
		// SpriteComponent->Sprite = CSprText.Object;
		SpriteComponent->SpriteInfo.Category = ID_Info;
		SpriteComponent->SpriteInfo.DisplayName = ID_Name;
		SpriteComponent->bIsScreenSizeScaled = true;
	}
	// setting spatially loaded to false could break datalayer usage which is critical
#endif // WITH_EDITORONLY_DATA

	Root->SetMobility(EComponentMobility::Static);
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
			const UWorld* const W = GetWorld();
			if (LIKELY(W)) W->GetTimerManager().SetTimerForNextTick(this, &APuzzle::Reset);
		}
		return;
	}

	if (IsValid(DoneInter)) {
		DoneInter->Locked = false; // force unlock
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
