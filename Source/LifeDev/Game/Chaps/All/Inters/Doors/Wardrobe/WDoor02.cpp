// Copyright Jerónimo Barraco-Mármol

#include "WDoor02.h"

#include "Components/AudioComponent.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

AWDoor02::AWDoor02():Super() {
	SFX->SetRelativeLocation(FVector(-30.753022,5.302253,110));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Wardrobe00/Wardrobe00-Door2.Wardrobe00-Door2"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-145,55,0));

	Interact->SetRelativeLocation(FVector(121.933455,-54.950261,107.080533));
	Interact->SetBoxExtent(FVector(22,5,96.697084));

	Anim->IsAdditive = true;
	Anim->TEnd.SetLocation(FVector::ZeroVector);
	Anim->TEnd.SetRotation(FRotator(0,-90,0).Quaternion());
}
