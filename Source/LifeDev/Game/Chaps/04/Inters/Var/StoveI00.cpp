// Copyright Jerónimo Barraco-Mármol

#include "StoveI00.h"

#include "PotI00.h"
#include "LifeDev/Core/Consts/ConstItems.h"

AStoveI00::AStoveI00():Super() {
	Locked = true;
	IsOneShot = true;
	TriggerDlg = "Stove00_T";
	LockedDlg = "Stove00_L";
	LockedItemDlg = "Stove00_LI";
	ULockItem = LDConsts::Items::Matches00;
	RewardFlash = .11;
	RewardStep = true; // advance the story
	RewardIntersActiveClass = {APotI00::StaticClass()};
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Pot/Boiling_Water_in_a_Coffee_Pot"));
	SFX_Trigger = CSnd.Object;
	Super::SetAutoActivate(false); // activated by LNPCI06
}
