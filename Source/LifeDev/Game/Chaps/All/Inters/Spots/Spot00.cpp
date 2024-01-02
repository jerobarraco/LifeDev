// Copyright Jerónimo Barraco-Mármol

#include "Spot00.h"

#include "Diags/Diags.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

// TODO make a subclass ASpot01 and replace the scene one with that one. then remove the specifics from here.
ASpot00::ASpot00():Super() {
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Clouts.Clouts"));
	SFX_Trigger = CSnd.Object;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Inters/Clouts/Shirt01.Shirt01"));
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
	// DropDlg = TEXT("IS_C0D");
	// TriggerDlg = "IS_C0T*";
	// LockedDlg = "IS_C0TB";
	Items = {
		// two shirts and a boxer, and a sock
		// FName("C1C00"), FName("C1C01"), FName("C1C02"), FName("C1C04")
	};
}