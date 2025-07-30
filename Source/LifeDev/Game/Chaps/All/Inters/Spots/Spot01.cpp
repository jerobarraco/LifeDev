// Copyright Jerónimo Barraco-Mármol

#include "Spot01.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ASpot01::ASpot01():Super() {
	UseAnim = false;
	UseRewardDestroy = false;
	Texts = {
		FText(NSLOCTEXT("Chap01", "Spot01.DropHere", "Drop clothes here")),
		FText(NSLOCTEXT("Chap01", "Spot01.Full", "All done")),
	};

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Clouts.Clouts"));
	// SFXTrigger = CSnd.Object;
	SFXs = {CSnd.Object, CSnd.Object};

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Inters/Clouts/Shirt00.Shirt00"));
	if (LIKELY(ObjMesh.Succeeded())) Mesh->SetStaticMesh(ObjMesh.Object);

	Mesh->SetRelativeLocation(FVector(-17.500000,17.500000,-0.000000));
	Interact->SetRelativeLocation(FVector(17.500000,-17.500000,5.0));
	Interact->SetBoxExtent(FVector(20.000000,20.000000,9.000000));
	Mesh->SetCastAllShadows(true);
}
