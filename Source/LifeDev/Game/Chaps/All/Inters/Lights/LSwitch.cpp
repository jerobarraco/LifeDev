// Copyright Jerónimo Barraco-Mármol

#include "LSwitch.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"

ALSwitch::ALSwitch():Super() {
	UseFade = false;
	UseRewardDestroy = false;

	/// anim
	UseAnim = true;
	// objects
	// TODO adjust on the level

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Fluorescent/Support"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));
	Mesh->SetRelativeRotation(FRotator(0,270,90));
	Mesh->SetRelativeScale3D(FVector(1,1,1));
	
	Interact->SetRelativeLocation(FVector(2.500000,-2.500000,0));
	Interact->SetBoxExtent(FVector(42.500000,7.500000,5.000000));
}
