// Copyright Jerónimo Barraco-Mármol

#include "Drawer02.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"

ADrawer02::ADrawer02():Super() {
	SFX->SetRelativeLocation(FVector(60.000000,0.000000,20.000000));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Cabinet00/Cabinet00-drawer1.Cabinet00-drawer1"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(80.000000,-62.500000,0.000000));
	Interact->SetRelativeLocation(FVector(-20.000000,62.5,20.000000));
	Interact->SetBoxExtent(FVector(5.000000,19.000000,5.000000));
}
