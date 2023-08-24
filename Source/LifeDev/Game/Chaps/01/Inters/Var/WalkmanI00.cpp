// Copyright Jerónimo Barraco-Mármol

#include "WalkmanI00.h"
#include "Interact/CInteract.h"

AWalkmanI00::AWalkmanI00():Super() {
	AnimEnabled = false;
	Interact->SetEnabled(true);
	
	TriggerFlashInc = .1f;
	TriggerDlg = "WM00_T"; // TODO
	LockedDlg = "WM00_L"; // TODO WM_WrongItem
	ItemReward = "WM00";// TODO
	
	Locked = true;
	ULockItemReq = "Batts"; // TODO Batteries
	Texts = {
		FText::FromString(TEXT("My walkman, with no batteries")),
	};
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Walkman00/Walkman00.Walkman00"));

	Interact->SetRelativeLocation(FVector(7,-5,1.725000));
	Interact->SetBoxExtent(FVector(7.5,5,2));
	// TODO set up meshes

	// snd /Script/Engine.SoundCue'/Game/LifeDev/Game/Chaps/All/Inters/Walkman00/Tape_SC.Tape_SC'
}

