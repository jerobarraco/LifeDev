// Copyright Jerónimo Barraco-Mármol

#include "TvI00.h"

ATvI00::ATvI00():Super() { }

void ATvI00::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	// TriggerDlg = IsEven() ? "": "TV00_T";
}
