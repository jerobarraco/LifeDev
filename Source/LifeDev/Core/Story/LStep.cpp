// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStep.h"

#include "Diags/Diags.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"

#include "LifeDev/Game/Sys/LGGameMode.h"

void ALStep::Start_Implementation() {
	Super::Start_Implementation();
	UWorld* const W = GetWorld();
	if (!W) return;

	EnsureItems(); // make sure items are awarded
	
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

	// check items. do here to avoid possibly finishing the step while it's starting.
	if (!ItemsFinish.IsEmpty()) {
		Inventory->OnMod.AddUniqueDynamic(this, &ALStep::ItemMod);
		
		// ensure to check if we already have the item
		UWorld* const W = GetWorld();
		if (W) {
			W->GetTimerManager().SetTimerForNextTick(this, &ALStep::CheckItemsFinish);
		}
	}
}

void ALStep::StartDialogs() {
	if (DlgId.IsNone()) return;

	Dialogs->AddId(DlgId);
	FinishAfterDlgs();
}

void ALStep::FinishAfterDlgs() {
	if (!Dialogs->GetIsShowing()) {
		Finish();
		return;
	}
	Dialogs->OnDone.AddUniqueDynamic(this, &ALStep::Finish);
}

void ALStep::RemoveItems() {
	for (const FName& N: ItemsRem) {
		Inventory->Rem(N);
	}
}

void ALStep::EnsureItems() {
	for(const FName& N: ItemsEnsure) {
		Inventory->Ensure(N);
	}
}

void ALStep::ItemMod(const FName& ItemName, int32 Diff, const FItem& Item) {
	CheckItemsFinish();
}

void ALStep::CheckItemsFinish() {
	const int32 NumItems = ItemsFinish.Num();
	if (NumItems<=0) return;

	for (int32 i=0; i<NumItems; ++i) {
		if (!Inventory->Has(ItemsFinish[i])) return;
	}

	FinishAfterDlgs();
}

void ALStep::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (!World) return;
	Dialogs = World->GetSubsystem<UDiags>();
	Inventory = World->GetSubsystem<UInventory>();
	FB = World->GetSubsystem<UFlashback>();
	Flags = UFlags::Instance(World);
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
