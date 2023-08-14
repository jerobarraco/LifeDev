// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "AnimTracks.h"
void UAnimTrackBase::Update(float Alpha) {}

void UAnimTrackMatF::Update(float Alpha) {
	Super::Update(Alpha);
	if (!IsValid(Mat)) return;
	const float V = FMath::Lerp(Start, End, Alpha);
	Mat->SetScalarParameterValue(Name, V);
}
