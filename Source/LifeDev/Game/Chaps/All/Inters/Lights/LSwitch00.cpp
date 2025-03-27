// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LSwitch00.h"

#include "Interact/Animator/CAnimatorMix.h"

ALSwitch00::ALSwitch00(): Super() {
	Anim->TStart = FTransform::Identity;
	Anim->TEnd.SetLocation(FVector(-2.5,0,0));
}
