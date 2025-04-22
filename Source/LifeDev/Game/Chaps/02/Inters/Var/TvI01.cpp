// Copyright Jerónimo Barraco-Mármol

#include "TvI01.h"

#include "Diags/Diags.h"

#include "LifeDev/Game/Sys/LGGameMode.h"

ATvI01::ATvI01():Super() {
	IsOneShot = true;
	RewardFlash = .15;
	UseRewardDestroy = false;
	UseFade = false;
}

void ATvI01::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	const AGameModeBase* const GameMode = World->GetAuthGameMode();
	const ALGGameMode* const GM = Cast<ALGGameMode>(GameMode);
	if (UNLIKELY(!GM)) return;

	GM->SetTempInputEnabled(false);
	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, this, &ATvI01::AfterOn, 5, false);
}

void ATvI01::AfterOn() {
	Diags->OnDone.AddUniqueDynamic(this, &ATvI01::AfterDlg);
	Diags->AddId("TV01_T");
}

void ATvI01::AfterDlg() {
	Diags->OnDone.RemoveAll(this);
	SetState(0);
}
