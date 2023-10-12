// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "CLNoiser.h"

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

void UCLNoiser::SetFB(float Value) {
	// note that when the fb goes up, the times and dist goes down
	DistMax = FMath::Lerp(DistFBMax, DistFBMin, Value);
	TimeMax = FMath::Lerp(TimeFBMax, TimeFBMin, Value);
}

void UCLNoiser::BeginPlay() {
	Super::BeginPlay();
	UFlashback* const F = UFlashback::Get(GetWorld());
	if (F) {
		F->OnChange.AddUniqueDynamic(this, &UCLNoiser::SetFB);
	}
}

void UCLNoiser::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UFlashback* const F = UFlashback::Get(GetWorld());
	if (F) {
		F->OnChange.RemoveAll(this);
	}
	
	Super::EndPlay(EndPlayReason);
}
