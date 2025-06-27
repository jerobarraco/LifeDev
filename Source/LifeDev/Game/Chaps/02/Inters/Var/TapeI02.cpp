// Copyright Jerónimo Barraco-Mármol

#include "TapeI02.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ATapeI02::ATapeI02():Super() {
	UseAutoActivate = true;
	UseAnim = false;
	UseFade = true;
	UseHint = true;
	StateNum = 1;
	Texts = { NSLOCTEXT("TapeI02","State0","Another of my tapes") };
	UseRewardDestroy = true;
	RewardFlash = .1;
	// note: find a way to not have to reward the tape. it's confusing.
	// but i need it because that's how the step is advanced. happen at different times.
	RewardItem = LDConsts::Items::Tape2;

	IsLocked = true;
	// important to require batteries since im going to use it directly on c2s3
	UnlockItems = { LDConsts::Items::Batts };
	LockedDlg = FName("T02_L"); // todo autodialog

	// LockedItemDlg = FName("T02_LI"); // will hint to use the batts
	// TriggerDlg = uses autodiag // will say what it is. triggered on pick up
	// Don't trigger the dialog here since it will trigger in the step, otherwise it will break the step
	// needed to be able to attach to the drawer
	Super::SetMobility(EComponentMobility::Movable);
	// At the moment it says the whole song on look or pick.
	// TODO leave only the 1st dialog as look or pick. then show the rest on item usage.
}
