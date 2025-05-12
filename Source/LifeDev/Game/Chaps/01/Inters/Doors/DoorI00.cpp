// Copyright Jerónimo Barraco-Mármol

#include "DoorI00.h"

ADoorI00::ADoorI00():Super() {
	UseAnim = false;
	IsLocked = true;
	RootComponent->SetWorldLocation(FVector(-324,-170,0));
	RootComponent->SetWorldRotation(FRotator(0.000000,89.999999,0.000000));
}
