// Copyright Jerónimo Barraco-Mármol

#include "DrawerI00.h"

// warning. this is used in multiple places.
// at least ch00 besides the bed.

void ADrawerI00::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	TriggerDlg = IsEven() ? "": "DR00_T";
}
