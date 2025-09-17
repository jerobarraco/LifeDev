// Copyright Jerónimo Barraco-Mármol

#include "Window.h"

#include "Interact/Animator/CAnimatorMix.h"

AWindow::AWindow():Super() {
	// TODO fill up. this is the base class for all windows (in general)
	//  Set the anim root to the root component so that i can attach items to it
	Anim->TEnd.SetRotation(FRotator(0, 90,0).Quaternion());
	Anim->TEnd.SetLocation(FVector::ZeroVector);
	Anim->IsAdditive = true;
}
