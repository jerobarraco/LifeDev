// Copyright Jerónimo Barraco-Mármol

#include "Drawer01.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ADrawer01::ADrawer01():Super() {
	UseAutoActivate = true;
	SFX->SetRelativeLocation(FVector(60,0,5));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Cabinets/Cabinet00-drawer0.Cabinet00-drawer0"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(80.000000,-62.500000,0.000000));
	Interact->SetRelativeLocation(FVector(-20.000000,62.5,5.000000));
	Interact->SetBoxExtent(FVector(5,18,5.5));
	SFX->SetRelativeLocation(FVector(5.000000,0.000000,2.500000));

	// read note of anim->troot on parent
}
