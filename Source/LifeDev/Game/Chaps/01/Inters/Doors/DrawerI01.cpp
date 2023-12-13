// Copyright Jerónimo Barraco-Mármol

#include "DrawerI01.h"

void ADrawerI01::SetState_Implementation(int32 NewState) {
	Super::SetState_Implementation(NewState);
	TriggerDlg = IsClosed() ? "": "DR01_T";
}
