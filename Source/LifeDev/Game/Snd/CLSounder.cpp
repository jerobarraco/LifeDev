// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "CLSounder.h"

#include "Sound/SoundClass.h"

#include "LifeDev/Game/Sys/Consts/ConstSettings.h"

UCLSounder::UCLSounder():Super() {
	static ConstructorHelpers::FObjectFinder<USoundClass>
		CSClass(LDConsts::Audio::Classes::SFX);
	SoundClassOverride = CSClass.Object;

	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		CAtt(LDConsts::Audio::Attns::SFX);
	AttenuationSettings = CAtt.Object;
}
