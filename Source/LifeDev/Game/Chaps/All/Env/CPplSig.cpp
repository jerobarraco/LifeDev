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
	DistanceSqr = { // unnecessary but...
		{ESigValue::Off, 200*200},
		{ESigValue::High, 201*201}
	};
	SetDefaultSignificance(ESigValue::Off);
}

ESigValue UCPplSig::CalcPplSig(const FTransform& Viewpoint) {
	// it has to be at a certain distance

	if (!Origin) return ESigValue::Off;

	const FVector& OrgLoc = Origin->GetComponentLocation();
	const float DistSqr = FVector::DistSquared(OrgLoc, Viewpoint.GetLocation());
	if (DistSqr < 200*200) return ESigValue::Off;

	// TODO not looking

	return ESigValue::High; // show
}

void UCPplSig::BeginPlay() {
	Super::BeginPlay();
	CalcSignificance.BindDynamic(this, &UCPplSig::CalcPplSig);
}
