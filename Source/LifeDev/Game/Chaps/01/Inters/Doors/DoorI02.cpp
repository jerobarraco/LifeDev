// Copyright Jerónimo Barraco-Mármol

#include "DoorI02.h"

ADoorI02::ADoorI02():Super() {
	UseAnim = false;
	IsLocked = true;
	RootComponent->SetWorldLocation(FVector(-290,960,0));
	RootComponent->SetWorldRotation(FRotator(0,180,0));
}
