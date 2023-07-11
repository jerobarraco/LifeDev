// Copyright Jerónimo Barraco-Mármol

#include "Door01.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"

// TODO make and inherit this from ADoor01I01 
ADoor01::ADoor01():Super() {
	// move to where this belongs (or delete)
	// RootComponent->SetWorldLocation(FVector(-325.010101,-170.010101,0.010105));
	// RootComponent->SetWorldRotation(FRotator(0.010101,89.999999,0.010101));

	// TODO set correct values for everything
	// TODO fix the door fpivot and fix it on door 00 too 
	IRoot->SetRelativeLocation(FVector(0,-5,0));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door01/Door01.Door01"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,5,0));
	SFX->SetRelativeLocation(FVector(-112.5,0,105));

	Interact->SetRelativeLocation(FVector(-60,-7.5,101));
	Interact->SetBoxExtent(FVector(60,5,101));
}
