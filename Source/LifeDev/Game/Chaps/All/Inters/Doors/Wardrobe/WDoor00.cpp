// Copyright Jerónimo Barraco-Mármol

#include "WDoor00.h"

#include "Components/AudioComponent.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

AWDoor00::AWDoor00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Wardrobe00/Door0"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-10,55,0));

	Interact->SetRelativeLocation(FVector(32.5,-55,107.5));
	Interact->SetBoxExtent(FVector(22,5,97.5));
	SFX->SetRelativeLocation(FVector(22.5,0,5));

	Anim->IsAdditive = true;
	Anim->TEnd.SetLocation(FVector::ZeroVector);
	Anim->TEnd.SetRotation(FRotator(0.0,90.0,0.0).Quaternion());
}
