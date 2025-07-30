#include "LSpot.h"

ALSpot::ALSpot():Super() {
	// always locked. we don't want it to trigger because that gives the reward.
	// it will trigger automatically
	IsLocked = true;
	IsOneShot = true; // disable after use
	UseAnim = false;
	UseRewardDestroy = false; // spots always reward stuff. but don't disappear.
	UseFade = false;
	UseStateLoop = false; // lock on the last state, since it might trigger more time than the num of states.
	StateNum = 2; // open and done
	Texts = {
		NSLOCTEXT("LSpot", "State0", "Drop Here"),
		NSLOCTEXT("LSpot", "State0", "Full"),
	};
	Super::SetMobility(EComponentMobility::Static); // spots don't need to move, by default.
}
