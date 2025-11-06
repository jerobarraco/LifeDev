// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol


#include "CPplSig.h"

// note, this is using the significance system in a weird way. so read this.
// it leverages the sig system for polling this component async.
// it becomes significant once you pass a couple of conditions
// It starts insignificant

UCPplSig::UCPplSig() {
	SetAutoActivate(true);
	IsOffIfHidden = false; // we are actually
	TestOcclusion = false;
	OffscreenTimeMax = -1;
	
}

ESigValue UCPplSig::CalcPplSig(const FTransform& Viewpoint) {
	return ESigValue::Off;
}

void UCPplSig::BeginPlay() {
	Super::BeginPlay();
	CalcSignificance.BindDynamic(this, &UCPplSig::CalcPplSig);
}
