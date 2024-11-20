// Copyright Jerónimo Barraco-Mármol

#include "WDoor02.h"

#include "Components/AudioComponent.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

AWDoor02::AWDoor02():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Wardrobe00/Door2"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-145,55,0));

	Interact->SetRelativeLocation(FVector(122.5,-55,107.5));
	Interact->SetBoxExtent(FVector(22,5,97.5));
	SFX->SetRelativeLocation(FVector(-22.5,0,5));

	Anim->IsAdditive = true;
	Anim->TEnd.SetLocation(FVector::ZeroVector);
	Anim->TEnd.SetRotation(FRotator(0,-90,0).Quaternion());
}
