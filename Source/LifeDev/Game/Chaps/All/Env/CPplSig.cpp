// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol


#include "CPplSig.h"

// note, this is using the significance system in a weird way. so read this.
// it leverages the sig system for polling this component async.
// it becomes significant once you pass a couple of conditions
// It starts insignificant

constexpr float _PplDist = 150;
constexpr float _PplDistSq = _PplDist*_PplDist;

UCPplSig::UCPplSig() {
	SetAutoActivate(false);
	IsOffIfHidden = false; // the owner is hidden by default.
	TestOcclusion = false;
	OffscreenTimeMax = -1;
	DistanceSqr = { // unnecessary but.... the CalcPplSig overrides this
		{ESigValue::Off, _PplDistSq},
		{ESigValue::High, _PplDistSq+10}
	};
	TickIntervals.Empty(); // don't mess with the intervals, otherwise the animations won't play
	SetDefaultSignificance(ESigValue::Off);
}

ESigValue UCPplSig::CalcPplSig(const FTransform& Viewpoint) {
	// it has to be at a certain distance

	UE_LOG(LogTemp, Log, TEXT("%hs o=%s"), __func__, *GetNameSafe(GetOwner()));
	if (!Origin) return ESigValue::Off;

	const FVector& OrgLoc = Origin->GetComponentLocation();
	const FVector& Line = OrgLoc - Viewpoint.GetLocation();
	const float DistSqr = Line.SquaredLength();
	// const float DistSqr = FVector::DistSquared(OrgLoc, Viewpoint.GetLocation());
	if (DistSqr < _PplDistSq) return ESigValue::Off;

	// TODO not looking
	
	return ESigValue::High; // show
}

void UCPplSig::BeginPlay() {
	Super::BeginPlay();

	CalcSignificance.BindDynamic(this, &UCPplSig::CalcPplSig);
	SetSignificance(ESigValue::Off);
}
