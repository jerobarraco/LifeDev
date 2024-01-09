// Copyright Jerónimo Barraco-Mármol

#include "Puzzle.h"

#include "Interact.h"
#include "Interact/CPuzzle.h"

APuzzle::APuzzle():Super() {
	CPuzzle = CreateDefaultSubobject<UCPuzzle>(TEXT("CPuzzle"));
	GetRootComponent()->SetMobility(EComponentMobility::Static);
}

void APuzzle::Done_Implementation(bool IsOk) {
	UE_LOG(LogTemp, Log, TEXT("APuzzle::Done ok=%i o=%s"), IsOk, *GetNameSafe(this));
	if (!IsOk || !IsValid(Interact)) return;
	Interact->TryTrigger();
}

void APuzzle::BeginPlay() {
	Super::BeginPlay();

	CPuzzle->OnDone.AddUniqueDynamic(this, &APuzzle::Done);
	CPuzzle->OnUpdate.AddUniqueDynamic(this, &APuzzle::Update);
}

void APuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (CPuzzle) {
		CPuzzle->OnDone.RemoveAll(this);
		CPuzzle->OnUpdate.RemoveAll(this);
	}
}
