// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01I00.h"

#include "Story/Story.h"

ALNPC01I00::ALNPC01I00():Super() {
	LockedDlg = "N01.L";
	Texts = { NSLOCTEXT("CH01", "NPC01I00", "Looks dangerous...") };
	UseFade = true;
	UseStateLoop = false;
	UseAutoActivate = true;
	UseRewardDestroy = false;
}

void ALNPC01I00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(Story)) Story->OnStart.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
}

void ALNPC01I00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	StandUp();
}

void ALNPC01I00::StandUp() {
	SetPoseStand();
	AddActorLocalRotation(FRotator(0, -120, 0)); // turn around
}
