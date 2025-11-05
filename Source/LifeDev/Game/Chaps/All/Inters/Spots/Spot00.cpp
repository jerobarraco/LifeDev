// Copyright Jerónimo Barraco-Mármol

#include "Spot00.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ASpot00::ASpot00():Super() {
	Texts = {
		NSLOCTEXT("Spot0", "DropHere", "Drop clothes here"),
		NSLOCTEXT("Spot0", "Full", "All done"),
	};
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Clouts.Clouts"));
	// SFXTrigger = CSnd.Object;
	SFXs = {CSnd.Object, CSnd.Object};

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Inters/Clouts/Shirt01.Shirt01"));
	if (LIKELY(ObjMesh.Succeeded())) Mesh->SetStaticMesh(ObjMesh.Object);

	Mesh->SetRelativeLocation(FVector(-17.500000,22.500000,0));
	Interact->SetRelativeLocation(FVector(17.500000,-22.500000,7.5));
	Interact->SetBoxExtent(FVector(18,23,10));
	Mesh->SetUseDynShadow(true);
}