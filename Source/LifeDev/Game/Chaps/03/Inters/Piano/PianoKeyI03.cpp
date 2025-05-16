// Copyright Jerónimo Barraco-Mármol

#include "PianoKeyI03.h"

APianoKeyI03::APianoKeyI03():Super() {
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Group_D"));
	// SFX_Trigger = CSFX.Object;
	SFXs = { CSFX.Object };
}
