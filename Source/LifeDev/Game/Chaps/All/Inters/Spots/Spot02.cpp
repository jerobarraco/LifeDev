// Copyright Jerónimo Barraco-Mármol

#include "Spot02.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ASpot02::ASpot02():Super() {
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Clouts.Clouts"));
	SFX_Trigger = CSnd.Object;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Inters/Clouts/Shirt02.Shirt02"));
	if (ObjMesh.Succeeded()) {
		Mesh->SetStaticMesh(ObjMesh.Object);
	}

	Mesh->SetRelativeLocation(FVector(-17.502020,22.502020,0));
	Interact->SetRelativeLocation(FVector(17.502020,-22.502020,7.5));
	Interact->SetBoxExtent(FVector(18,23,10));
	Texts = {
		FText(NSLOCTEXT("Chap02", "Spot02.DropHere", "Drop clothes here")),
		FText(NSLOCTEXT("Chap02", "Spot02.Full", "All done")),
	};
	// DropDlg = TEXT("IS_C0D");
	// TriggerDlg = "IS_C0T*";
	// LockedDlg = "IS_C0TB";
	Items = {
		// two shirts and a boxer, and a sock
		// FName("C1C02"), FName("C1C02"), FName("C1C02"), FName("C1C04")
	};
}