// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCI03.h"

#include "Interact/Animator/CAnimatorFade.h"

ALNPCI03::ALNPCI03():Super() {
	UseFade = true;
	AnimFade->Duration = 3; // needs to be slower due to camera movement
	SetActorHiddenInGame(true);
}
