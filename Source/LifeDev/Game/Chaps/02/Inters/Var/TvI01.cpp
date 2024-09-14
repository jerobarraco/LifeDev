// Copyright Jerónimo Barraco-Mármol

#include "TvI01.h"

#include "Diags/Diags.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/LGGameMode.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ATvI01::ATvI01():Super() {
	UseItemDlgs = {
		{"Batts", "TV01xBatts"},
		{LDConsts::Items::Card0, "TV01xC00"},
		{LDConsts::Items::Card1, "TV01xC01"},
		{LDConsts::Items::Card2, "TV01xC02"},
	};
}

void ATvI01::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	Locked = true;

	UWorld* const World = GetWorld();
	if (!World) return;

	AGameModeBase* const GameMode = World->GetAuthGameMode();
	ALGGameMode* const GM = Cast<ALGGameMode>(GameMode);
	if (!GM) return;

	GM->SetTempInputEnabled(false);
	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, this, &ATvI01::AfterOn, 5, false);
	UFlashback* const FB = UFlashback::Instance(World);
	if (FB) {
		FB->ModVal(.15, 3*(1/.15)); // 3 secs
	}
}

void ATvI01::AfterOn() {
	Diags->OnDone.AddUniqueDynamic(this, &ATvI01::AfterDlg);
	Diags->AddId("TV01_T");
}

void ATvI01::AfterDlg() {
	Diags->OnDone.RemoveAll(this);
	SetState(0);
}
