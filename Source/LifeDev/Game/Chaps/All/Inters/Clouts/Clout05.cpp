// Copyright Jerónimo Barraco-Mármol

#include "Clout05.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"
#include "JUtils/Misc/JMiscConsts.h"

AClout05::AClout05():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Inters/Clouts/Sock_P"));
	IFL(ObjMesh.Succeeded())
		Mesh->SetStaticMesh(ObjMesh.Object);
	
	Mesh->SetRelativeLocation(FVector(-10.000000,10.000000,-2.500000));
	Interact->SetRelativeLocation(FVector(10.000000,-10.000000,2.500000));
	Interact->SetBoxExtent(FVector(10.000000,10.000000,2.500000));
	// sock_p has a good palette material set already, so no need to load it.
	// but we need to clear the one from the animfade so that it uses the one from the mesh.
	AnimFade->MatBase = nullptr;
}
