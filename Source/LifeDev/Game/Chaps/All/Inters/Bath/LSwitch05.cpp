// Copyright Jerónimo Barraco-Mármol

#include "LSwitch05.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

ALSwitch05::ALSwitch05():Super() {
	const FVector Loc(-7.500000,32.500000,-77.500000);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Bath/Toilet02/Btn2")); // already grey
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(Loc);
	
	Anim->CopyTStart();
	Anim->TEnd.SetLocation(FVector(0, 0, -2.5));

	Interact->SetRelativeLocation(FVector(7.500000,-32.500000,77.500000));
	Interact->SetBoxExtent(FVector(2.5)); // luckily with this button size it doesn't protrude that much
}
