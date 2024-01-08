// Copyright Jerónimo Barraco-Mármol

#include "Puzzle.h"

#include "Diags/Diags.h"
#include "Interact/CPuzzle.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "Story/Story.h"

APuzzle::APuzzle():Super() {
	CPuzzle = CreateDefaultSubobject<UCPuzzle>(TEXT("CPuzzle"));
	GetRootComponent()->SetMobility(EComponentMobility::Static);
}

void APuzzle::Done_Implementation(bool IsOk) {
	UE_LOG(LogTemp, Log, TEXT("APuzzle::Done ok=%i o=%s"), IsOk, *GetNameSafe(this));
}

void APuzzle::BeginPlay() {
	Super::BeginPlay();
	UWorld* const W = GetWorld();
	if (!W) return;
	
	Story = UStory::Instance(W);
	FB = UFlashback::Instance(W);
	Flags = UFlags::Instance(W);
	Diags = UDiags::Instance(W);
	Inventory = UInventory::Instance(W);

	CPuzzle->OnDone.AddUniqueDynamic(this, &APuzzle::Done);
	CPuzzle->OnUpdate.AddUniqueDynamic(this, &APuzzle::Update);
}

void APuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	if (CPuzzle) {
		CPuzzle->OnDone.RemoveAll(this);
		CPuzzle->OnUpdate.RemoveAll(this);
	}
	
	Flags = nullptr;
	FB = nullptr;
	Diags = nullptr;
	Inventory = nullptr;
	Story = nullptr;
}
