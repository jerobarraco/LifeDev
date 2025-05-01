// Copyright Jerónimo Barraco-Mármol

#include "WCabDoor01.h"

#include "Components/AudioComponent.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

AWCabDoor01::AWCabDoor01():Super() {
	SFX->SetRelativeLocation(FVector(0,-55,32.5));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Cabinets/Cabinet01-DoorL.Cabinet01-DoorL"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-75,5,-10));

	Interact->SetRelativeLocation(FVector(75,-35,42.5));
	Interact->SetBoxExtent(FVector(7.5,30,32.5));
	SFX->SetRelativeLocation(FVector(6.,-20.,0.));

	Anim->TEnd.SetRotation(FRotator(0, 90, 0).Quaternion());
}
