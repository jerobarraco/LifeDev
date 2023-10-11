// Copyright (C) 2023 Jeronimo Barraco-Marmol


#include "CLNoiser.h"

#include "LifeDev/Game/Flashback/Flashback.h"


UCLNoiser::UCLNoiser() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UCLNoiser::SetFB(float Value) {
	
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
