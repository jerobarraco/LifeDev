// Copyright Jerónimo Barraco-Mármol

#include "Drawer.h"

#include "Interact/Animator/CAnimatorMix.h"

ADrawer::ADrawer():Super() {
	// Set the anim root to the root component so that i can attach items to it.
	// Anim is set to TRoot to RootComp to allow to attach stuff to it.
	// but that means the Tend is always in word coords depending on rotation. and it sucks.
	Anim->TRoot = RootComponent;
	Anim->TEnd.SetLocation(FVector(40, 0,0));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CDrawer (TEXT("/Game/LifeDev/Game/Inters/Generic/Drawers/Drawer_C.Drawer_C"));
	SFXs = {CDrawer.Object, CDrawer.Object};
	SFX_Stop.Empty();
}

void ADrawer::BeginPlay() {
	Super::BeginPlay();
	Anim->CopyTStart();
}
