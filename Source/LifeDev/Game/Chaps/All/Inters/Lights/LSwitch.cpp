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
		CMesh (TEXT("Game/LifeDev/Game/Inters/Tv00/Tv00-Btn"));
	Mesh->SetStaticMesh(CMesh.Object);
	
	// Interact->SetRelativeLocation(FVector(32.5,-22.5,25));
	Interact->SetBoxExtent(FVector(35,35,25));
}
