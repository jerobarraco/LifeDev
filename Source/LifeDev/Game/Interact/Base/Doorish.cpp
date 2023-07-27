// Copyright Jerónimo Barraco-Mármol

#include "Doorish.h"

#include "Interact/Animator/CAnimatorMix.h"


ADoorish::ADoorish():Super() {
	AnimEnabled = true;
	Anim->TRoot = RootComponent;
	Anim->IsAdditive = true;

	static ConstructorHelpers::FObjectFinder<USoundBase>
		SOpen(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/SBvfe1_Door_Handle_D_002.SBvfe1_Door_Handle_D_002"));
	SFX_Open = SOpen.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SClose(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/door-02.door-02"));
	SFX_CloseEnd = SClose.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SLocked(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/door_lock_007.door_lock_007"));
	SFX_Locked = SLocked.Object;
}
