// Copyright Jerónimo Barraco-Mármol

#include "DrawerI01.h"

void ADrawerI01::Trigger_Implementation() {
	TriggerDlg = IsOpen() ? "" : "DR01_T";
	Super::Trigger_Implementation();
}
