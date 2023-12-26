// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStep.h"

#include "Diags/Diags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"

#include "LifeDev/Game/Sys/LGGameMode.h"

void ALStep::Start_Implementation() {
	Super::Start_Implementation();
	UWorld* const W = GetWorld();
	if (!W) return;

	EnsureItems();
	if (!FinishItems.IsEmpty()) {
		Inventory->OnMod.AddUniqueDynamic(this, &ALStep::ItemMod);

		// ensure to check if we already have the item
		// don't do just now since the child of this class would get confused as stop will trigger before start
		W->GetTimerManager().SetTimerForNextTick(this, &ALStep::CheckFinishItems);
	}
	
	AGameModeBase* const GameModeBase = W->GetAuthGameMode();
	ALGGameMode* const LGGameMode = Cast<ALGGameMode>(GameModeBase);
	// ALGGameMode* const LGGameMode = ALGGameMode::Get(); // doesn't work
	if (!IsValid(LGGameMode)) return;
	LGGameMode->SetCharInputEnabled(InputEnabled);
}

void ALStep::Stop_Implementation() {
	if (IsValid(Dialogs)) {
		Dialogs->OnDone.RemoveAll(this);
	}
	if (IsValid(Inventory)) {
		Inventory->OnMod.RemoveAll(this);
	}
	if (IsValid(FB)) {
		FB->OnChange.RemoveAll(this);
	}

	UWorld* const W = GetWorld();
	if (W) {
		// ensure we don't double trigger
		W->GetTimerManager().ClearAllTimersForObject(this);
	}

	RemoveItems();
	
	Super::Stop_Implementation();
}

void ALStep::PostWait_Implementation() {
	Super::PostWait_Implementation();
	// otherwise show dialogs
	StartDialogs();
}

void ALStep::StartDialogs() {
	if (DlgId.IsNone()) return;

	Dialogs->OnDone.AddUniqueDynamic(this, &ALStep::Finish);
	FDialogSequence Seq; TArray<FDialog> Diags; TArray<FDialogChar> Chars;
	if (!Dialogs->AddId(DlgId)) {
		// if it fails to add it, then finish manually
		Finish();
	}
}

void ALStep::RemoveItems() {
	FItem Item; // not reusing since some items might change the count.
	for (const FName& N: RemItems) {
		Item.Count = 0; // ensure it's 0 to begin with
		const bool Has = Inventory->Get(N, Item);
		if (!Has || Item.Count <= 0) continue;
		Inventory->Mod(N, -Item.Count);
	}
}

void ALStep::EnsureItems() {
	for(const FName& N: ItemsEnsure) {
		Inventory->Ensure(N);
	}
}

void ALStep::ItemMod(const FName& ItemName, int32 Diff, const FItem& Item) {
	CheckFinishItems();
}

void ALStep::CheckFinishItems() {
	const int32 NumItems = FinishItems.Num();
	if (NumItems<=0) return;
	// if (Diff<=0) return; // this is causing issues. todo fix

	for (int32 i=0; i<NumItems; ++i) {
		if (!Inventory->Has(FinishItems[i])) return;
	}
	// if i have all the items. schedule a finish
	// wait for dialogs to end
	Dialogs->OnDone.AddUniqueDynamic(this, &ALStep::Finish);
	FTimerHandle Handle;
	
	UWorld* const World = GetWorld();
	if (!World) return;
	// also set a time out if the player takes too long.
	World->GetTimerManager().SetTimer(Handle, this, &ALStep::Finish, FadeTime*2);
}

void ALStep::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (!World) return;
	Dialogs = World->GetSubsystem<UDiags>();
	Inventory = World->GetSubsystem<UInventory>();
	FB = World->GetSubsystem<UFlashback>();
}

void ALStep::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (IsValid(Dialogs)) {
		Dialogs->OnDone.RemoveAll(this);
	}
	Dialogs = nullptr;

	if (IsValid(Inventory)) {
		Inventory->OnMod.RemoveAll(this);
	}
	Inventory = nullptr;
	
	if (IsValid(FB)) {
		FB->OnChange.RemoveAll(this);
	}
	FB = nullptr;
	// always at end
	Super::EndPlay(EndPlayReason);
}

void ALStep::PostLoad() {
	Super::PostLoad();
	if (UseFadeTime) {
		WaitTime = FadeTime; 
	}
	
	// avoid finishing earlier if we have dialogs
	if (!DlgId.IsNone()) {
		FinishPostWait = false;
	}
}

void ALStep::Finish_Implementation() {
	// avoid possible double triggering.
	Dialogs->OnDone.RemoveDynamic(this, &ALStep::Finish);
	Super::Finish_Implementation();
}
