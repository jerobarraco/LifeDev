// Copyright Jerónimo Barraco-Mármol

#include "Door00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"

// TODO make door 01
// TODO inherit this from ADoor00I00 
ADoor00::ADoor00():Super() {
	// move to where this belongs (or delete)
	// RootComponent->SetWorldLocation(FVector(-325.000000,-170.000000,0.000005));
	// RootComponent->SetWorldRotation(FRotator(0.000000,89.999999,0.000000));

	IRoot->SetRelativeLocation(FVector(0,-5,0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/Door00.Door00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,5,0));
	SFX->SetRelativeLocation(FVector(-112.5,0,105));

	Interact->SetRelativeLocation(FVector(-60,-7.5,100));
	Interact->SetBoxExtent(FVector(60,5,100));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CFrameMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/Door00-Frame.Door00-Frame"));
	Frame->SetStaticMesh(CFrameMesh.Object);
	Frame->SetRelativeLocation(FVector(0,-10,0));
	Frame->SetRelativeRotation(FRotator(0,180,0).Quaternion());
}
