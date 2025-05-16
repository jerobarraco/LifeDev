// Copyright Jerónimo Barraco-Mármol

#include "PianoKeyI01.h"

APianoKeyI01::APianoKeyI01():Super() {
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Group_B"));
	// SFX_Trigger = CSFX.Object;
	SFXs = { CSFX.Object };
}
