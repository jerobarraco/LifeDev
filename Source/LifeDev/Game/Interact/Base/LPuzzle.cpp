// Copyright Jerónimo Barraco-Mármol

#include "LPuzzle.h"

#include "Diags/Diags.h"
#include "Interact/CPuzzle.h"
#include "Interact/Interact.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"

#include "Story/Story.h"

#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Interact/LInteract.h"

void ALPuzzle::SetUseItemDlgs(const TMap<FName, FName>& Dlgs) {
	if (!CPuzzle) return;

	// set the dialogs on each registered interact
	TArray<AInteract*> Inters = CPuzzle->GetInteracts();
	for (AInteract* const I: Inters) {
		ALInteract* const LI = Cast<ALInteract>(I);
		if (!IsValid(LI)) continue;
		// better to override the whole array than having issues down the line.
		// it also allows to remove stuff.
		LI->UseItemDlgs = Dlgs;	
	}
}

void ALPuzzle::Done_Implementation(bool IsOk) {
	UE_LOG(LogTemp, Log, TEXT("ALPuzzle::Done ok=%i o=%s"), IsOk, *GetNameSafe(this));

	Super::Done_Implementation(IsOk); // triggers the interact AND RESETS (next frame)
	
	if (!IsOk) return; // ok to skip super on not ok, since super doesn't care

	/// do all rewardy stuff
	
	if (Diags) Diags->AddId(DoneDlg);
	if (FB) FB->ModVal(DoneFB);
	if (Flags) Flags->Mod(DoneFlag, 1); // intentionally ADDING one (not setting to one)
	if (Inventory) Inventory->Mod(DoneItem, 1); // intentionally ADDING one (not setting to one)
	if (Story && !DoneStep.IsNone()) Story->StartNext(DoneStep);

	// fade if it's an L interact (those can fade)
	// a bit yucky but better than subclassing cpuzzle. it's actually quite the best option.
	ALInteract* const Reward = Cast<ALInteract>(DoneActor);
	if (IsValid(Reward)) {
		Reward->Fade(true);
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
}

void ALPuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	Flags = nullptr;
	FB = nullptr;
	Diags = nullptr;
	Inventory = nullptr;
	Story = nullptr;
}
