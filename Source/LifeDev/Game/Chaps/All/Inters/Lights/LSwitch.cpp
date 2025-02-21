// Copyright Jerónimo Barraco-Mármol

#include "LSwitch.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

ALSwitch::ALSwitch():Super() {
	UseFade = false;
	UseRewardDestroy = false;
	UseAnim = true;
	// TODO adjust on the level

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("Game/LifeDev/Game/Inters/Tv00/Tv00-Btn"));
	Mesh->SetStaticMesh(CMesh.Object);
	
	// Interact->SetRelativeLocation(FVector(32.5,-22.5,25));
	Interact->SetBoxExtent(FVector(35,35,25));

	Anim->Duration = .5;
	Anim->IsAdditive = true;
	Anim->TEnd.SetLocation(FVector(0.0,-2.5,0));
}
