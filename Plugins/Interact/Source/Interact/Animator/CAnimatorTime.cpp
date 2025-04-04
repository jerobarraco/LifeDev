// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "CAnimatorTime.h"

UCAnimatorTime::UCAnimatorTime() {
	UseTimeDilation = false;
	SetComponentTickInterval(0); // to be more accurate. (default is 30fps)

	// this is the most usable for stop-action
	Duration = .001;
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/Niagara/DefaultAssets/Curves/Templates/RampUpDown"));
	if (LIKELY(CCurve.Succeeded())) Curve = CCurve.Object;
}

void UCAnimatorTime::Activate(const bool bReset) {
	Super::Activate(bReset);

	if (LIKELY(UseFromCurrent && (bReset || !IsActive()))) {
		const UWorld* const World = GetWorld();
		if (UNLIKELY(!World)) return;

		const AWorldSettings* const Settings =
			World->GetWorldSettings(false, false);
		if (UNLIKELY(!Settings)) return;

		From = Settings->TimeDilation;
	}
}

void UCAnimatorTime::Update_Implementation(const float Alpha) {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	AWorldSettings* const Settings =
		World->GetWorldSettings(false, false);
	if (UNLIKELY(!Settings)) return;

	Super::Update_Implementation(Alpha);
	const float NewDilation = FMath::LerpStable(From, To, Alpha);
	Settings->SetTimeDilation(NewDilation); // has a clamp. also future-proof.
	Dilation = Settings->TimeDilation;// get the actual one for the future.
}
