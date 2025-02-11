// Copyright Jerónimo Barraco-Mármol

#include "Spot00.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ASpot00::ASpot00():Super() {
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Clouts.Clouts"));
	SFX_Trigger = CSnd.Object;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Inters/Clouts/Shirt01.Shirt01"));
	if (ObjMesh.Succeeded()) Mesh->SetStaticMesh(ObjMesh.Object);

	Mesh->SetRelativeLocation(FVector(-17.500000,22.500000,0));
	Interact->SetRelativeLocation(FVector(17.500000,-22.500000,7.5));
	Interact->SetBoxExtent(FVector(18,23,10));
	Texts = {
		FText(NSLOCTEXT("Chap01", "Spot00.DropHere", "Drop clothes here")),
		FText(NSLOCTEXT("Chap01", "Spot00.Full", "All done")),
	};
	Mesh->SetCastAllShadows(true);
	// DropDlg = TEXT("IS_C0D");
	// TriggerDlg = "IS_C0T*";
	// LockedDlg = "IS_C0TB";
	// LockedFullDlg = "IS_C0TBF";
	// Items = { };
}