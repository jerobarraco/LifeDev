// Copyright Jerónimo Barraco-Mármol

#include "Drawer03.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ADrawer03::ADrawer03():Super() {
	UseAutoActivate = true;
	IsTEndRelative = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Arch/Cabinets/Cabinet02-Drawer"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-5.000000,5.000000,-15.000000));
	Interact->SetRelativeLocation(FVector(75.000000,-50.000000,22.500000));
	Interact->SetBoxExtent(FVector(5.000000,45.000000,7.500000));
	SFX->SetRelativeLocation(FVector(5.000000,0.000000,2.500000));
}
