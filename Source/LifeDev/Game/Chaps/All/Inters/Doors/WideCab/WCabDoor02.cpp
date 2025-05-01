// Copyright Jerónimo Barraco-Mármol

#include "WCabDoor02.h"

#include "Components/AudioComponent.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

AWCabDoor02::AWCabDoor02():Super() {
	SFX->SetRelativeLocation(FVector(0,55,32.5));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Cabinets/Cabinet01-DoorR.Cabinet01-DoorR"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-75,130,-10));
	Interact->SetRelativeLocation(FVector(75,-100,42.5));
	SFX->SetRelativeLocation(FVector(6.,20.,0.));

	Anim->TEnd.SetRotation(FRotator(0, -90, 0).Quaternion());
}
