// Copyright Jerónimo Barraco-Mármol

#include "MatchBoxI00.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "LifeDev/Core/Consts/ConstItems.h"

AMatchBoxI00::AMatchBoxI00():Super() {
	RewardItem = LDConsts::Items::Matches00;
	UseRewardDestroy = true;
	UseFade = true;
	// Locked = true;
	TriggerDlg = "Matches00_T";
	// LockedDlg = "Matches00_L";
	RewardFlash = .1; // story works better this way
	AnimFade->SetNewMat();
	Super::SetAutoActivate(false); // activated by the npci06
}
