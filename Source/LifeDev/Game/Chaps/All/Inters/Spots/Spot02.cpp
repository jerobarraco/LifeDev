// Copyright Jerónimo Barraco-Mármol

#include "Spot02.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ASpot02::ASpot02():Super() {
	Texts = { FText(NSLOCTEXT("Chap02", "Spot02.Sit", "Sit here")) };

	// TODO sound
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Clouts"));
	// SFXTrigger = CSnd.Object;
	SFXs = {CSnd.Object, CSnd.Object};
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Arch/Chairs/Chair00"));
	if (LIKELY(ObjMesh.Succeeded())) Mesh->SetStaticMesh(ObjMesh.Object);

	Mesh->SetRelativeLocation(FVector(-17.5,17.5,0));
	SFX->SetRelativeLocation(FVector(17.5,-17.5,40));
	Interact->SetRelativeLocation(FVector(17.5,-17.5,40));
	Interact->SetBoxExtent(FVector(17.5, 17.5, 40));
	// DropDlg = TEXT("IS_C0D");
	// LockedDlg = "IS_C0TB";
	// Items = {};
}