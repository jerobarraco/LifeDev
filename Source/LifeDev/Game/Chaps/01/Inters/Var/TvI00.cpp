// Copyright Jerónimo Barraco-Mármol

#include "TvI00.h"

void ATvI00::SetState_Implementation(int32 NewState) {
	Super::SetState_Implementation(NewState);
	TriggerDlg = IsOpen() ? "TV00_T": "";
}