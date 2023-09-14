// Copyright Jerónimo Barraco-Mármol

#include "WDoor01.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

AWDoor01::AWDoor01():Super() {
	SFX->SetRelativeLocation(FVector(-30.753022,5.302253,110));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Wardrobe00/Wardrobe00-Door1.Wardrobe00-Door1"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-95,60,0));
	Interact->SetRelativeLocation(FVector(72.513521,-55,107.5));
	Interact->SetBoxExtent(FVector(22,5,97.5));
	Anim->IsAdditive = true;
	Anim->TEnd.SetLocation(FVector::ZeroVector);
	Anim->TEnd.SetRotation(FRotator(0.0,-90.0,0.0).Quaternion());
}
