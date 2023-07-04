// Copyright Jerónimo Barraco-Mármol

#include "Drawer.h"

#include "Interact/Animator/CAnimatorMix.h"

ADrawer::ADrawer():Super() {
	//  Set the anim root to the root component so that i can attach items to it
	Anim->TRoot = RootComponent;
	Anim->TEnd.SetLocation(FVector(40, 0,0));
}
