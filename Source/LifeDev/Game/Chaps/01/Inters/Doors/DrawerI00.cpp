// Copyright Jerónimo Barraco-Mármol

#include "DrawerI00.h"

void ADrawerI00::Trigger_Implementation() {
	TriggerDlg = GetIsOpen() ? "" : "DR00_T";
	Super::Trigger_Implementation();
}
