// Copyright Jerónimo Barraco-Mármol

#include "PianoKeyI00.h"

APianoKeyI00::APianoKeyI00():Super() {
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Group_A"));
	// SFX_Trigger = CSFX.Object;
	SFXs = { CSFX.Object };
}
