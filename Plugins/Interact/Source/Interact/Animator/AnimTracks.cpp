// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "AnimTracks.h"
void FAnimTrackMatV::Update(float Alpha) {
	// UE_LOG(LogTemp, Log, TEXT("Track preupdate %3.3f %3.3f"), T, Alpha);
	if (!IsValid(Mat)) return;
	
	// UE_LOG(LogTemp, Log, TEXT("Track Update '%s' %3.3f %3.3f"), *Name.ToString(), T, Alpha);
	const FLinearColor V = FLinearColor::LerpUsingHSV(Start, End, Alpha);
	Mat->SetVectorParameterValue(Name, V);
}

void FAnimTrackMatF::Update(float Alpha) {
	// UE_LOG(LogTemp, Log, TEXT("Track preupdate %3.3f %3.3f"), T, Alpha);
	if (!IsValid(Mat)) return;
	
	// UE_LOG(LogTemp, Log, TEXT("Track Update '%s' %3.3f %3.3f"), *Name.ToString(), T, Alpha);
	const float V = FMath::LerpStable(Start, End, Alpha);
	Mat->SetScalarParameterValue(Name, V);
}

void FAnimTrackTrans::Update(float Alpha) {
	if (!IsValid(Root)) return;

	FTransform TNew = Start;
	if (IsAdditive) {
		TNew.Accumulate(TNew, (const ScalarRegister) Alpha);
		// TStart.BlendFromIdentityAndAccumulate(TNew, TEnd, (const ScalarRegister) Alpha);
		// TNew.Accumulate(TEnd, (const ScalarRegister) Alpha); // not what i want, does something different with the scale.
	}else {
		// Thanks Tim! this actually works very well!
		TNew.BlendWith(End, Alpha);
	}
	Root->SetRelativeTransform(TNew);
}


void FAnimTrackTrans::Init() {
	if (IsValid(Root) && IsAdditive) {
		Start = Root->GetRelativeTransform();
	}
}

void UCAnimTracks::BeginPlay() {
	Super::BeginPlay();
	for (FAnimTrackTrans& F: Transfs) {
		F.Init();
	}
}

void UCAnimTracks::Update_Implementation(float Alpha) {
	Super::Update_Implementation(Alpha);

	for (FAnimTrackMatF& F: MatFs) {
		F.Update(Alpha);
	}
	for (FAnimTrackMatV& F: MatVs) {
		F.Update(Alpha);
	}
	for (FAnimTrackTrans& F: Transfs) {
		F.Update(Alpha);
	}
}
