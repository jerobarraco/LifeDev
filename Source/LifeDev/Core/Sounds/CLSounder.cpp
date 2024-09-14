// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "CLSounder.h"

#include "Sound/SoundClass.h"

#include "LifeDev/Core/Consts/ConstSettings.h"

UCLSounder::UCLSounder():Super() {
	static ConstructorHelpers::FObjectFinder<USoundClass>
		CSClass(LDConsts::Audio::Classes::SFX);
	SoundClassOverride = CSClass.Object;

	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		CAtt(LDConsts::Audio::Attns::SFX);
	AttenuationSettings = CAtt.Object;
	// unnecessary, but if someone checks "bOverrideAttenuation" it will try to use this.
	AttenuationOverrides = AttenuationSettings->Attenuation;
}
