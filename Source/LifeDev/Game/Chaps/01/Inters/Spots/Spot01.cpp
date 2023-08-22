// Copyright Jerónimo Barraco-Mármol

#include "Spot01.h"

#include "Interact/CInteract.h"
#include "JUtils/CQuickMesh.h"

ASpot01::ASpot01():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Chaps/1/Inters/Clothes/Shirt00.Shirt00"));
	if (ObjMesh.Succeeded()) {
		Mesh->SetStaticMesh(ObjMesh.Object);
	}

	Mesh->SetRelativeLocation(FVector(-17.500000,17.500000,-0.000000));
	Interact->SetRelativeLocation(FVector(17.500000,-17.500000,5.0));
	Interact->SetBoxExtent(FVector(20.000000,20.000000,9.000000));
	Texts = {
		FText(NSLOCTEXT("Chap01", "Spot01.DropHere", "Drop her clothes here")),
		FText(NSLOCTEXT("Chap01", "Spot01.Full", "All done")),
	};
	TriggerDlg = FName("IS_C1T");
	LockedDlg = "IS_C1TB";
	DropDlg = "IS_C1D";
	Items = {FName("C1C03")}; // the bra
}
