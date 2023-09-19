// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "Flags.h"

#include "Engine/DataTable.h"

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
