// Copyright Jerónimo Barraco-Mármol

#include "Drawer03.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ADrawer03::ADrawer03():Super() {
	UseAutoActivate = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Arch/Cabinets/Cabinet02-Drawer"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(80.000000,-62.500000,0.000000));
	Interact->SetRelativeLocation(FVector(-20.000000,62.5,20.000000));
	Interact->SetBoxExtent(FVector(5.000000,19.000000,5.000000));
	SFX->SetRelativeLocation(FVector(5.000000,0.000000,2.500000));
}
