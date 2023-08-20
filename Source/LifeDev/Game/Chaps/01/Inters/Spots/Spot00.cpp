// Copyright Jerónimo Barraco-Mármol

#include "Spot00.h"

#include "Interact/CInteract.h"

ASpot00::ASpot00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjMesh(TEXT("/Game/LifeDev/Game/Chaps/1/Inters/Clothes/Shirt01.Shirt01"));
	if (ObjMesh.Succeeded()) {
		Mesh->SetStaticMesh(ObjMesh.Object);
	}

	Mesh->SetRelativeLocation(FVector(-17.500000,22.500000,0));
	Interact->SetRelativeLocation(FVector(17.500000,-22.500000,7.5));
	Interact->SetBoxExtent(FVector(18,23,10));
	Texts = {
		FText(NSLOCTEXT("Chap01", "Spot00.DropHere", "Drop clothes here")),
		FText(NSLOCTEXT("Chap01", "Spot00.Full", "All done")),
	};
	DropDlg = TEXT("IS_C0D");
	TriggerDlg = FName("IS_C0T");
	TriggerBadDlg = "IS_C0TB";
	Items = {
		// two shirts and a boxer, and a sock
		FName("C1C00"), FName("C1C01"), "C1C02",  "C1C04"
	};
}
