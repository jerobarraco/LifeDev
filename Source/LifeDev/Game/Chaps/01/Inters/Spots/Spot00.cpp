// Copyright Jerónimo Barraco-Mármol

#include "Spot00.h"

#include "Diags/Diags.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ASpot00::ASpot00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Clouts/Shirt01.Shirt01"));
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
	TriggerDlg = "IS_C0T"; // TODO use a IS_C0T* (note the random)
	LockedDlg = "IS_C0TB";
	Items = {
		// two shirts and a boxer, and a sock
		FName("C1C00"), FName("C1C01"), FName("C1C02"), FName("C1C04")
	};
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Clouts/Clouts.Clouts"));
	SFX_Trigger = CSnd.Object;
}

EItemUseResult ASpot00::TryUseItem_Implementation(const FName& Name) {
	if (Name == LDConsts::Items::Bra) {
		Dialogs->AddId("IS_C0_C03");
		return EItemUseResult::BAD_HANDLED;
	}
	return Super::TryUseItem_Implementation(Name);
}
