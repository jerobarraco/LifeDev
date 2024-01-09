// Copyright Jerónimo Barraco-Mármol

#include "LPuzzle.h"

#include "Diags/Diags.h"
#include "Interact/CPuzzle.h"
#include "Interact/Interact.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"

#include "Story/Story.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALPuzzle::ALPuzzle():Super() {
	CPuzzle = CreateDefaultSubobject<UCPuzzle>(TEXT("CLPuzzle"));
	GetRootComponent()->SetMobility(EComponentMobility::Static);
}

void ALPuzzle::Done_Implementation(bool IsOk) {
	UE_LOG(LogTemp, Log, TEXT("ALPuzzle::Done ok=%i o=%s"), IsOk, *GetNameSafe(this));
	if (!IsOk) return;

	if (Diags) {
		Diags->AddId(DoneDlg);
	}

	if (IsValid(Interact)) {
		// force unlock
		Interact->Locked = false;
		Interact->TryTrigger();
	}
}

void ALPuzzle::BeginPlay() {
	Super::BeginPlay();
	UWorld* const W = GetWorld();
	if (!W) return;
	
	Story = UStory::Instance(W);
	FB = UFlashback::Instance(W);
	Flags = UFlags::Instance(W);
	Diags = UDiags::Instance(W);
	Inventory = UInventory::Instance(W);

	CPuzzle->OnDone.AddUniqueDynamic(this, &ALPuzzle::Done);
	CPuzzle->OnUpdate.AddUniqueDynamic(this, &ALPuzzle::Update);
}

void ALPuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
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
