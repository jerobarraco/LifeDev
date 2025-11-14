// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol


#include "CPplSig.h"

#include "Kismet/KismetMathLibrary.h"

// note, this is using the significance system in a weird way. so read this.
// it leverages the sig system for polling this component async.
// it becomes significant once you pass a couple of conditions
// It starts insignificant

constexpr float _PplDist = 175;
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
	// UE_LOG(LogTemp, Verbose, TEXT("%hs o=%s"), __func__, *GetNameSafe(GetOwner()));
	UE_LOG(LogTemp, Verbose, TEXT("%hs o=%s begin=%f wait=%f"), __func__, *GetNameSafe(GetOwner()),
		BeginTime, WaitTime);
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World) | !Origin) return ESigValue::Off; // required

	// force to wait some time
	const double Time = World->GetTimeSeconds();
	if ((BeginTime+WaitTime) > Time) return ESigValue::Off;
	
	// check distance. don't show if too close
	const FVector& OrgLoc = Origin->GetComponentLocation();
	const FVector& Line = OrgLoc - Viewpoint.GetLocation();
	const float DistSqr = Line.SquaredLength();
	// const float DistSqr = FVector::DistSquared(OrgLoc, Viewpoint.GetLocation());
	if (DistSqr < _PplDistSq) return ESigValue::Off;

	/// check the angle. don't show if looking at it.
	// rotation that correspond to look directly at the orgloc (it's on the feet btw)
	const FRotator& LookRot = UKismetMathLibrary::FindLookAtRotation(Viewpoint.GetLocation(), OrgLoc);
	const FRotator& ViewRot = Viewpoint.Rotator(); // current view rotation
	FRotator DifRot = ViewRot - LookRot; // angle between looking at it, and current
	DifRot.Normalize();

	// const float RX = DifRot.GetComponentForAxis(EAxis::Type::X); // unnecessary, always 0
	const float RY = DifRot.GetComponentForAxis(EAxis::Type::Y);
	const float RZ = DifRot.GetComponentForAxis(EAxis::Type::Z);
	UE_LOG(LogTemp, Verbose, TEXT("%hs rot y=%.5f z=%.5f o=%s"), __func__, RY, RZ,
		*GetNameSafe(GetOwner()));

	// the object is visible in this range y (up/down) -37 to 93 // z (left/right) -65 to 65
	if (((RZ>-65) & (RZ<65)) & ((RY>-40) & (RY<95))) return ESigValue::Off; // looking at it vertically. notice only check this AFTER the horizontal

	return ESigValue::High; // show
}

void UCPplSig::BeginPlay() {
	Super::BeginPlay();

	CalcSignificance.BindDynamic(this, &UCPplSig::CalcPplSig);
	SetSignificance(ESigValue::Off);

	// in theory is ok to get these numbers her since this instance is loaded per chapter.
	// so this begin play will be executed when the chapter starts
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	BeginTime = World->GetTimeSeconds();
	WaitTime = FMath::FRandRange(WaitTimeMin, WaitTimeMax);
}
