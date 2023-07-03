// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "MuseActor.h"

# include "Muse.h"

void AMuseActor::SetEEG_Implementation(const TArray<float>& Eeg) {
	FString EEGS = "Got EEG: ";
	for (float f: Eeg) {
		EEGS += FString::Printf(TEXT("%3.2f, "), f);
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *EEGS);
}

void AMuseActor::SetBands_Implementation(int32 BandNum, float Avg, const TArray<float>& Vals) {
	FString Band = FString::Printf(TEXT("Got Band [%d] Avg=%3.2f Vals="), BandNum, Avg);
	
	for (float f: Vals) {
		Band += FString::Printf(TEXT("%3.2f, "), f);
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *Band);
}

void AMuseActor::SetConfig_Implementation() {
	System->SetAddress("0.0.0.0", 5000);
	System->UseEEG = true;
	System->UseAlpha = System->UseBeta = System->UseGamma = System->UseDelta = System->UseTheta = true;
}

void AMuseActor::BeginPlay() {
	Super::BeginPlay();
	System = GetWorld()->GetSubsystem<UMuse>();
	SetConfig();
	System->OnEEG.AddUniqueDynamic(this, &AMuseActor::SetEEG);
	System->OnBands.AddUniqueDynamic(this, &AMuseActor::SetBands);
	System->Start();
}

void AMuseActor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (IsValid(System)) {
		System->OnEEG.RemoveAll(this);
		System->OnBands.RemoveAll(this);
		System->Stop();
	}
	Super::EndPlay(EndPlayReason);
}
