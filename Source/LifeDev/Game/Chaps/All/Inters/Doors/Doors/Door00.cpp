// Copyright Jerónimo Barraco-Mármol

#include "Door00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ADoor00::ADoor00():Super() {
	IRoot->SetRelativeLocation(FVector(0,-5,0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Doors/Door00/Door00.Door00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,7.5,0));
	SFX->SetRelativeLocation(FVector(-112.5,0,105));

	Interact->SetRelativeLocation(FVector(-60,-7.5,100));
	Interact->SetBoxExtent(FVector(60,10,100));

	Anim->TEnd.SetRotation(FRotator(0, -90, 0).Quaternion());
}
