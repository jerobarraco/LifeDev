// Copyright Jerónimo Barraco-Mármol

#include "PianoKeyI02.h"

APianoKeyI02::APianoKeyI02():Super() {
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Group_C"));
	// SFX_Trigger = CSFX.Object;
	SFXs = { CSFX.Object };
}
