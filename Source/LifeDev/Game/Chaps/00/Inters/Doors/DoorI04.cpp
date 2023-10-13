// Copyright Jerónimo Barraco-Mármol

#include "DoorI04.h"

#include "Interact/Animator/CAnimatorMix.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ADoorI04::ADoorI04():Super() {
	LockedDlg = "D04_LCK";
	ULockItemReq = LDConsts::Items::Poem0;
	Locked = true;
	Anim->TEnd.SetRotation(FRotator(0, -90, 0).Quaternion());
}
