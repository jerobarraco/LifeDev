// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCI00.h"

#include "Story/Story.h"

ALNPCI00::ALNPCI00():Super() {
	// LockedDlg = "N01.L"; LNPCI00
	Texts = { NSLOCTEXT("CH01", "NPC01I00", "Looks dangerous...") };
	UseFade = true;
	UseStateLoop = false;
	UseAutoActivate = true;
	UseRewardDestroy = false;
	SetActorHiddenInGame(true);
}

void ALNPCI00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(Story)) Story->OnStart.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
}

void ALNPCI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	StandUp();
}

void ALNPCI00::StandUp() {
	SetPoseStand();
	AddActorLocalRotation(FRotator(0, -120, 0)); // turn around
}
