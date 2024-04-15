// Copyright Jerónimo Barraco-Mármol

#include "Spot02.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ASpot02::ASpot02():Super() {
	// TODO sound
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Clouts"));
	SFX_Trigger = CSnd.Object;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Arch/Chairs/Chair00"));
	if (ObjMesh.Succeeded()) {
		Mesh->SetStaticMesh(ObjMesh.Object);
	}

	StateNum = 1;
	Mesh->SetRelativeLocation(FVector(-17.5,17.5,0));
	SFX->SetRelativeLocation(FVector(17.5,-17.5,40));
	Interact->SetRelativeLocation(FVector(17.5,-17.5,40));
	Interact->SetBoxExtent(FVector(17.5, 17.5, 40));
	Texts = {
		FText(NSLOCTEXT("Chap02", "Spot02.Sit", "Sit here")),
	};
	// DropDlg = TEXT("IS_C0D");
	// TriggerDlg = "IS_C0T*";
	// LockedDlg = "IS_C0TB";
	Items = {
		// two shirts and a boxer, and a sock
		// FName("C1C02"), FName("C1C02"), FName("C1C02"), FName("C1C04")
	};
}