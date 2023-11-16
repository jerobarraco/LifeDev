// Copyright Jerónimo Barraco-Mármol

#include "DrawerI00.h"

void ADrawerI00::SetState_Implementation(int32 NewState) {
	Super::SetState_Implementation(NewState);
	TriggerDlg = IsOpen() ? "DR00_T" : "";
}
