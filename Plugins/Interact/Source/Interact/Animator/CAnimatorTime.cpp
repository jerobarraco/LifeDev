// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "CAnimatorTime.h"

void UCAnimatorTime::Update_Implementation(const float Alpha) {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	AWorldSettings* const Settings = World->GetWorldSettings(false, false);
	if (UNLIKELY(!World )) return;

	Super::Update_Implementation(Alpha);
	const float NewDilation = FMath::LerpStable(From, To, Alpha);
	Settings->SetTimeDilation(NewDilation); // has a clamp. also future proof.
	Dilation = Settings->TimeDilation;// get the actual one for the future.
	if (FMath::IsNearlyZero(Dilation))
		Dilation = UE_SMALL_NUMBER; // don't crash
}

void UCAnimatorTime::TickComponent(float DeltaTime, const ELevelTick TickType,
	FActorComponentTickFunction* const ThisTickFunction) {
	DeltaTime /= Dilation;
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
