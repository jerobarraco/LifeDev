// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#include "Flags.h"

UFlags* UFlags::Instance(UWorld* W) {
	if (!IsValid(W)) return nullptr;
	UFlags* const I = W->GetSubsystem<UFlags>();
	return IsValid(I) ? I : nullptr;
}

void UFlags::Mod(const FName& Name, float Diff) {
	if (Name.IsNone()) return;

	const float Val = Get(Name) + Diff; 
	if (FMath::IsNearlyZero(Val)) {
		Flags.Remove(Name);
	} else {
		Flags.Add(Name, Val);
	}
	
	OnMod.Broadcast(Name, Diff, Val);
}

void UFlags::Set(const FName& Name, float Val) {
	// this is basically duplicated code... but. it will be faster than getting and mod'ing (for about one call to Get)
	if (Name.IsNone()) return;

	const float Old = Get(Name); // broadcasting the diff is what adds complexity here
	if (FMath::IsNearlyZero(Val)) {
		Flags.Remove(Name);
	} else {
		Flags.Add(Name, Val);
	}
	
	OnMod.Broadcast(Name, -Old, Val);
}
