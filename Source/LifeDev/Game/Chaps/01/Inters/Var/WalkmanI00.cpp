// Copyright Jerónimo Barraco-Mármol

#include "WalkmanI00.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AWalkmanI00::AWalkmanI00():Super() {
	AnimEnabled = false;
	Interact->SetEnabled(true);
	
	TriggerFlashInc = .1f;
	// TriggerDlg = "WM00_T"; // TODO maybe add a dialog when picked?
	LockedDlg = "WM_L";
	LockedItemDlg = "WM_LI"; // will trigger if i have the batteries. TODO add
	ULockItem = "Batts";
	
	ItemReward = "WM";
	ULockBadDlg = "WM_ULB";
		
	Locked = true;
	Texts = {
		FText::FromString(TEXT("My walkman, with no batteries")),
	};
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Walkman00/Walkman00.Walkman00"));
	Mesh->SetStaticMesh(ObjMesh.Object);
	
	Interact->SetRelativeLocation(FVector(7,-5,1.725000));
	Interact->SetBoxExtent(FVector(7.5,5,2));

	ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Walkman00/Tape_SC.Tape_SC"));
	SFX_Trigger = CSnd.Object;
}

