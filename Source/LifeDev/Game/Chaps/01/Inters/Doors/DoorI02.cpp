// Copyright Jerónimo Barraco-Mármol

#include "DoorI02.h"

ADoorI02::ADoorI02():Super() {
	UseAnim = false;
	LockedDlg = "D02_LCK";
	Locked = true;
	RootComponent->SetWorldLocation(FVector(-290,960,0));
	RootComponent->SetWorldRotation(FRotator(0,180,0));
}
