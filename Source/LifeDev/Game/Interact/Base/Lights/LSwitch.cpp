// Copyright Jerónimo Barraco-Mármol

#include "LSwitch.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

ALSwitch::ALSwitch():Super() {
	UseAnim = true;
	UseFade = false;
	UseRewardDestroy = false;
	UseStateLoop = true;
	UseAttachedSFX = true;
	StateNum = 2;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Tv00/Tv00-Btn"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-57.500000,2.500000,-12.500000));

	Interact->SetRelativeLocation(FVector(57.500000,-2.500000,12.500000));
	Interact->SetBoxExtent(FVector(5));

	Anim->Duration = .5;
	Anim->IsAdditive = true;
	Anim->TEnd.SetLocation(FVector(0.0,-2.5,0));
}
