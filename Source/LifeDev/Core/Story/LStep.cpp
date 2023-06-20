// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStep.h"

#include "Dialogs/Dialogs.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

void ALStep::Start_Implementation() {
	Super::Start_Implementation();
	AGameModeBase* const GameModeBase = GetWorld()->GetAuthGameMode();
	ALGGameMode* const LGGameMode = Cast<ALGGameMode>(GameModeBase);
	// ALGGameMode* const LGGameMode = ALGGameMode::Get();
	if (LGGameMode) {
		LGGameMode->SetCharInputEnabled(InputEnabled);
	}

	if (!FinishItems.IsEmpty()) {
		Inventory = GetWorld()->GetSubsystem<UInventory>();
		Inventory->OnMod.AddUniqueDynamic(this, &ALStep::ItemMod);
	}
}

void ALStep::Stop_Implementation() {
	Super::Stop_Implementation();
	Dialogs->OnDone.RemoveAll(this);
	if (IsValid(Inventory)) {
		Inventory->OnMod.RemoveAll(this);
	}
}

void ALStep::PostWait_Implementation() {
	Super::PostWait_Implementation();
	// otherwise show dialogs
	StartDialogs();
}

void ALStep::StartDialogs() {
	if (SeqId.IsNone()) return;
	Dialogs->OnDone.AddUniqueDynamic(this, &ALStep::Finish);
	FDialogSequence Seq; TArray<FDialog> Diags; TArray<FDialogChar> Chars;
	Dialogs->AddId(SeqId);
}

void ALStep::ItemMod(const FName& ItemName, int32 Diff, const FItem& Item) {
	const int32 NumItems = FinishItems.Num();
	if (NumItems<=0) return;
	if (Diff<=0) return;
	for (int32 i=0; i<NumItems; ++i) {
		if (!Inventory->Has(FinishItems[i])) return;
	}
	// if i have all the items. schedule a finish
	// wait for dialogs to end
	Dialogs->OnDone.AddUniqueDynamic(this, &ALStep::Finish);
	FTimerHandle Handle;
	// also set a time out if the player takes too long.
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ALStep::Finish, FadeTime*2);
}

void ALStep::BeginPlay() {
	Super::BeginPlay();
	Dialogs = GetWorld()->GetSubsystem<UDialogs>();
}

void ALStep::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Dialogs = nullptr;
	Inventory = nullptr;
	Super::EndPlay(EndPlayReason);
}

void ALStep::PostLoad() {
	Super::PostLoad();
	if (UseFadeTime) {
		WaitTime = FadeTime; 
	}
	
	// avoid finishing earlier if we have dialogs
	if (!SeqId.IsNone()) {
		FinishPostWait = false;
	}
}
