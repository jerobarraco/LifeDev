// Copyright Jerónimo Barraco-Mármol

#include "Puzzle.h"

#include "Diags/Diags.h"
#include "Interact/CPuzzle.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"

APuzzle::APuzzle():Super() {
	CPuzzle = CreateDefaultSubobject<UCPuzzle>(TEXT("CPuzzle"));
}

void APuzzle::BeginPlay() {
	Super::BeginPlay();
	UWorld* const W = GetWorld();
	FB = UFlashback::Get(W);
	Diags = UDiags::Get(W);
	Inventory = UInventory::Get(W);
}

void APuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	FB = nullptr;
	Diags = nullptr;
	Inventory = nullptr;
}
