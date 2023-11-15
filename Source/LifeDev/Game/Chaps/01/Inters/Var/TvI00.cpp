// Copyright Jerónimo Barraco-Mármol

#include "TvI00.h"

ATvI00::ATvI00():Super() {
	// TriggerDlg = "TV00_T";
}

void ATvI00::Trigger_Implementation() {
	TriggerDlg = IsOpen() ? "" : "TV00_T";
	Super::Trigger_Implementation();
}
