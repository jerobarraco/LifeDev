// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "CLNoiser.h"

#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Flashback/Flashback.h"

UCLNoiser::UCLNoiser():Super() {
	PrimaryComponentTick.bCanEverTick = false;

	TimeMin = 5;
	TimeFBMin = 10; // notice this is not exactly the same as min, otherwise it will get repetitive
	TimeFBMax = TimeMax = 55; // this is the same, that's fine. the fb will change the max
	DistMin = 10;
	DistFBMin = 15; // notice ont the same as min
	DistFBMax = DistMax = 500;
	HalfAngleWidth = (360.0-90.0)/2.0; // the back
	HalfAngleHeight = 40.0;
}

void UCLNoiser::Activate(bool bReset) {
	// don't activate if this is disabled
	if (!ULSettings::GetFeatS(GetWorld(), EFeat::S_NOISE)) return;
	Super::Activate(bReset);
}

void UCLNoiser::SetFB(float Value) {
	// note that when the fb goes up, the times and dist goes down
	DistMax = FMath::Lerp(DistFBMax, DistFBMin, Value);
	TimeMax = FMath::Lerp(TimeFBMax, TimeFBMin, Value);
}

void UCLNoiser::BeginPlay() {
	Super::BeginPlay();
	UWorld* const W = GetWorld();
	UFlashback* const F = UFlashback::Instance(W);
	if (F) {
		F->OnChange.AddUniqueDynamic(this, &UCLNoiser::SetFB);
	}

	ULSettings* const S = ULSettings::Instance(W);
	if (S) {
		Debug = S->GetFeat(EFeat::DBG_SOUND);
		S->OnFeatUpdateSound.AddUniqueDynamic(this, &UCLNoiser::FeatUpdate);
	}
}

void UCLNoiser::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UWorld* const W = GetWorld();
	if (!W) return;

	UFlashback* const F = UFlashback::Instance(W);
	if (F) {
		F->OnChange.RemoveAll(this);
	}

	ULSettings* const S = ULSettings::Instance(W);
	if (S) {
		S->OnFeatUpdateSound.RemoveAll(this);
	}
	
	Super::EndPlay(EndPlayReason);
}

void UCLNoiser::FeatUpdate(EFeat Feat, bool bEnabled) {
	if (Feat != EFeat::S_NOISE) return;
	SetActive(bEnabled, false);
}
