// Copyright Jerónimo Barraco-Mármol

#include "Drawer01.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"

ADrawer01::ADrawer01():Super() {
	SFX->SetRelativeLocation(FVector(60,0,5));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Cabinet00/Cabinet00-drawer0.Cabinet00-drawer0"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(80.000000,-63.000000,0.000000));
	Interact->SetRelativeLocation(FVector(-20.000000,62.538369,5.000000));
	Interact->SetBoxExtent(FVector(5.000000,18.265225,5.580736));
}
