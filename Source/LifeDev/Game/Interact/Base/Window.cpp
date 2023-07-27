// Copyright Jerónimo Barraco-Mármol

#include "Window.h"

#include "Interact/Animator/CAnimatorMix.h"

AWindow::AWindow():Super() {
	//  Set the anim root to the root component so that i can attach items to it
	// TODO fill up
	Anim->TEnd.SetRotation(FRotator(0, 90,0).Quaternion());
	Anim->TEnd.SetLocation(FVector::ZeroVector);
}
