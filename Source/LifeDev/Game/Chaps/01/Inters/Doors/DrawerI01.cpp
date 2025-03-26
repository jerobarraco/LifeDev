// Copyright Jerónimo Barraco-Mármol

#include "DrawerI01.h"


// warning. this is potentially used in multiple places.
// at least on ch00 besides the bed.

void ADrawerI01::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	TriggerDlg = IsEven() ? "": "DR01_T";
}
