// Copyright Jerónimo Barraco-Mármol

#include "Window01.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"

AWindow01::AWindow01():Super() {
	// TODO set up
	SFX->SetRelativeLocation(FVector(9,-55,43));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/WindowB/WindowB-SideA.WindowB-SideA"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,5,0));
	Interact->SetRelativeLocation(FVector(9,-35,60));
	Interact->SetBoxExtent(FVector(5,30,55));
	
	// TODO set positions
	// TODO set use parent bounds (what was the name? )
	// TODO make a child of ustaticmeshcompont that is more optimized
}
