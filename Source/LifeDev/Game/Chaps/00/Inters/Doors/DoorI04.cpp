// Copyright Jerónimo Barraco-Mármol

#include "DoorI04.h"

#include "Interact/Animator/CAnimatorMix.h"

ADoorI04::ADoorI04():Super() {
	LockedDlg = "D04_LCK";
	ULockItemReq = "P00";
	
	Locked = true;
	Anim->TEnd.SetRotation(FRotator(0, -90, 0).Quaternion());
}
