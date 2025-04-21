// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01I00.h"

#include "Story/Story.h"

ALNPC01I00::ALNPC01I00():Super() {
	LockedDlg = "N01.L";
	Texts = { NSLOCTEXT("CH01", "NPC01I00", "Looks dangerous...") };
	UseFade = true;
	UseStateLoop = false;
	UseAutoActivate = true;
}

void ALNPC01I00::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LIKELY(Story)) Story->OnStart.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
}

void ALNPC01I00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	
	StandUp();
}
//
// EItemUseResult ALNPC01I00::TryUseItem_Implementation(const FName& Name) {
// 	if (Name != LDConsts::Items::Card0) {
// 		const bool Handled = Diags->AddId("N01.IB");
// 		return Handled ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
// 	}
//
// 	// disable the interact, so it can fade better, and player won't trigger again
// 	SetActive(false);
// 	Story->OnStart.AddUniqueDynamic(this, &ALNPC01I00::DoStepStart);
// 	Story->StartNext(); // trigger next step (C1S5) (which has a dialog)
// 	return EItemUseResult::SUCCESS;
// }

void ALNPC01I00::StandUp() {
	SetPoseStand();
	AddActorLocalRotation(FRotator(0, -120, 0)); // turn around
}

void ALNPC01I00::DoStepStart(AStep* const Step) {
	if (UNLIKELY(!Step)) return;
	if (Step->Name == "C1S6") { // after the dialog
		StandUp();
	} else if (Step->Name == "C1S7") {
		Fade(false); // fade out manually. doRewards won't.
		DoRewards(); // give the card and disappear
		if (LIKELY(Story)) Story->OnStart.RemoveAll(this);
	}
}
