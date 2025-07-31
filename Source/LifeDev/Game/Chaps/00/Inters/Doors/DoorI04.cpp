// Copyright Jerónimo Barraco-Mármol

#include "DoorI04.h"

#include "Interact/Animator/CAnimatorMix.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ADoorI04::ADoorI04():Super() {
	// LockedDlg = "D04_LCK";
	// ULockItemReq = LDConsts::Items::Poem0;
	UnlockItems = {LDConsts::Items::Poem0};
	IsLocked = true;
	Anim->TEnd.SetRotation(FRotator(0, -90, 0).Quaternion());
	UseAutoActivate = true;
}
