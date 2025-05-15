// Copyright Jerónimo Barraco-Mármol

#include "HeaterI03.h"

AHeaterI03::AHeaterI03():Super() {
	UseAnim = false;
	UseFade = false;
	UseRewardDestroy = false;
	RewardFlash = .1;
	IsOneShot = true;
	IsLocked = false;
	UseAutoActivate = true;
	// LockedDlg = "HT03_L";
	TriggerDlg = "HT03_L"; // TODO rename
	Texts = { NSLOCTEXT("HeaterI00", "State0", "Turn on") };

	// TODO find a new one
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Env/Noises/squeaky_closet_door_open_001.squeaky_closet_door_open_001"));
	SFX_Trigger = CSnd.Object;
}
