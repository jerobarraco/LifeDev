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
	UE_LOG(LogTemp, Log, TEXT("UFlags::Mod: name=%s diff=%3.3f new=%3.3f"), *Name.ToString(), Diff, Val);
	
	if (FMath::IsNearlyZero(Val)) {
		Flags.Remove(Name);
	} else {
		Flags.Add(Name, Val);
	}
	
	OnMod.Broadcast(Name, Diff, Val);
}

void UFlags::Set(const FName& Name, float Val) {
	// this is basically duplicated code with mod...
	// But it will be faster than getting and mod'ing (for about one call to Get)
	if (Name.IsNone()) return;

	const float Old = Get(Name); // broadcasting the diff is what adds complexity here
	UE_LOG(LogTemp, Log, TEXT("UFlags::Set: name=%s old=%3.3f new=%3.3f"), *Name.ToString(), Old, Val);
	if (FMath::IsNearlyZero(Val)) {
		Flags.Remove(Name);
	} else {
		Flags.Add(Name, Val);
	}
	
	OnMod.Broadcast(Name, Val-Old, Val);
}

void UFlags::SetAll(const TMap<FName, float>& NewFlags) {
	Clear(NewFlags.Num());
	
	TArray<FName> Keys;
	NewFlags.GetKeys(Keys);

	for (const FName& K: Keys) {
		const float* pV = NewFlags.Find(K);
		if (!pV) continue;
		
		Set(K, *pV);
	}
}

void UFlags::Clear(int32 Reserve) {
	TArray<FName> Keys;
	Flags.GetKeys(Keys);
	for (const FName& K: Keys) {
		float* pV = Flags.Find(K);
		if (!pV) continue;
		
		OnMod.Broadcast(K, -*pV, 0.0);
	}

	Flags.Empty(Reserve);
}
