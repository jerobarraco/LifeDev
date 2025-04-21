// Copyright Jerónimo Barraco-Mármol

#include "StoveI00.h"

#include "PotI00.h"
#include "LifeDev/Core/Consts/ConstItems.h"

AStoveI00::AStoveI00():Super() {
	IsLocked = true;
	IsOneShot = true;
	TriggerDlg = "Stove00_T";
	LockedDlg = "Stove00_L";
	LockedItemDlg = "Stove00_LI";
	ULockItem = LDConsts::Items::Matches00;
	RewardFlash = .11;
	RewardStep = true; // advance the story TODO deprecated
	RewardIntersActiveClass = {APotI00::StaticClass()};
	// UseAutoActivate = false; // activated by LNPCI06 // redundant

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Pot/Boiling_Water_in_a_Coffee_Pot"));
	SFX_Trigger = CSnd.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndB(TEXT("/Game/LifeDev/Game/Inters/Kitchen/MatchBox/match_burn_meridian_-09"));
	SFX_TriggerB = CSndB.Object;
}

void AStoveI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();

	// small trick so that i can play 2 sfxs at once. the default one is shortlived.
	UseAttachedSFX = false;
	PlaySFX(SFX_TriggerB);
	UseAttachedSFX = true;
}
