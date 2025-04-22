// Copyright Jerónimo Barraco-Mármol

#include "TvI02.h"

static const FName NTriggerDlg = "TV02_T";
ATvI02::ATvI02():Super() {
	TriggerDlg = NTriggerDlg;
}

void ATvI02::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	// ==1 because it's set for the next interaction
	// only trigger when turning on
	TriggerDlg = NewState == 1 ? NTriggerDlg : NAME_None;
}
