// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "AnimTracks.h"
void UAnimTrackBase::Update(float T, float Alpha) {}

void UAnimTrackMatF::Update(float T, float Alpha) {
	Super::Update(T, Alpha);
	// UE_LOG(LogTemp, Log, TEXT("Track preupdate %3.3f %3.3f"), T, Alpha);
	if (!IsValid(Mat)) return;
	
	// UE_LOG(LogTemp, Log, TEXT("Track Update '%s' %3.3f %3.3f"), *Name.ToString(), T, Alpha);
	const float V = FMath::LerpStable(Start, End, Alpha);
	Mat->SetScalarParameterValue(Name, V);
}
