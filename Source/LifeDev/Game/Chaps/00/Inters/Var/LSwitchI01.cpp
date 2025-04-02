// Copyright Jerónimo Barraco-Mármol

#include "LSwitchI01.h"

#include "Interact/Animator/CAnimatorMix.h"

ALSwitchI01::ALSwitchI01() {
	IsOneShot = true;
	// only used for the ch0, kinda unnecessary, but...
	UseActiveOnce = true;
	UseAutoActivate = false;
	StateNum = 2;
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/Interact/Interact_C.Interact_C"));
	Anim->Curve = CCurve.Object;
}
