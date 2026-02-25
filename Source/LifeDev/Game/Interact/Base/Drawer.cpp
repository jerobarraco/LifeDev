// Copyright Jerónimo Barraco-Mármol

#include "Drawer.h"

#include "Interact/Animator/CAnimatorMix.h"

ADrawer::ADrawer():Super() {
	// Set the anim root to the root component so that i can attach items to it.
	// Anim is set to TRoot to RootComp to allow to attach stuff to it.
	// but that means the Tend is always in word coords depending on rotation. and it sucks.
	Anim->TRoot = RootComponent;
	Anim->TEnd.SetLocation(FVector(40, 0,0));

	// not sure if this works since the other meshes are attached to the root component :/
	// Mesh->SetLightAttachmentsAsGroup(true);

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CDrawer (TEXT("/Game/LifeDev/Game/Inters/Generic/Drawers/Drawer_C.Drawer_C"));
	SFXs = {CDrawer.Object, CDrawer.Object};
	SFX_Stop.Empty();
}

void ADrawer::BeginPlay() {
	Super::BeginPlay();
	// necessary because the drawers use the rootcomponent as troot, so the transform is set on the outliner.
	// TODO maybe one day i can implement something like i did with the AInteract::Label checking if TRoot == RootComponent.
	Anim->CopyTStart();
	
	// TODO once all objects in the scene has this to True, remove this variable (but keep the adjustment)
	if (IsTEndRelative) {
		// Rotate the offset and re-set
		Anim->TEnd.SetLocation(Anim->TStart.GetRotation().RotateVector(Anim->TEnd.GetLocation()));
		// you'd think this'll do the trick, but it doesn't.
		// Anim->TEnd.SetToRelativeTransform(Anim->TStart);
	}
}
