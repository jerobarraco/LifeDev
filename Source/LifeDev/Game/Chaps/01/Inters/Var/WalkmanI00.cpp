// Copyright Jerónimo Barraco-Mármol

#include "WalkmanI00.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

#include "LifeDev/Core/Consts/ConstItems.h"

AWalkmanI00::AWalkmanI00():Super() {
	UseAnim = false;
	
	RewardFlash = .1f;
	// TriggerDlg = "WM00_T"; // TODO maybe add a dialog when picked?
	RewardItem = "WM";
	UseRewardDestroy = true;
	UseFade = true;
	UseAutoActivate = true;

	// TODO allow to pick without the batteries. needs to change the door and stuff.
	// Locked = false;
	IsLocked = true;
	LockedDlg = "WM_L";
	// LockedItemDlg = "WM_LI"; // will trigger if i have the batteries. TODO add
	ULockItemReq = LDConsts::Items::Batts;
	// ULockCondition = "{V.Item.Count.Batts}"; // unlock if it has the batteries
	UseHint = true;
	// needed to break the loading dependency
	HintCondition = "{V.Inter.Trigger.WDoorI00}";
	Texts = {
		NSLOCTEXT("WalkmanI00", "State0", "My walkman, with no batteries")};
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Inters/Walkman00/Walkman00.Walkman00"));
	Mesh->SetStaticMesh(ObjMesh.Object);
	
	Interact->SetRelativeLocation(FVector(7,-5,1.725000));
	Interact->SetBoxExtent(FVector(7.5,5,2));

	ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Walkman00/Tape_SC.Tape_SC"));
	SFX_Trigger = CSnd.Object;
}

