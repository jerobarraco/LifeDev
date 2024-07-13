// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#include "Flags.h"

DEFINE_LOG_CATEGORY_STATIC(LogFlags, Log, Log);

UFlags* UFlags::Instance(UObject* O) {
	if (!IsValid(O)) return nullptr;
	
	const UWorld* const W = O->GetWorld();
	if (!IsValid(W)) return nullptr;

	UFlags* const I = W->GetSubsystem<UFlags>();
	return IsValid(I) ? I : nullptr;
}

void UFlags::Mod(const FName& Name, const float Diff, const bool Log) {
	if (Name.IsNone()) return;
	
	const float Val = Get(Name) + Diff; 
	if (Log) UE_LOG(LogFlags, Log, TEXT("%hs: name=%s diff=%3.3f new=%3.3f"),
		__func__, *Name.ToString(), Diff, Val);
	
	Flags.Add(Name, Val);
	OnMod.Broadcast(Name, Diff, Val);
}

void UFlags::Rem(const FName& Name) {
	if (Name.IsNone()) return;
	
	const float Val = Get(Name); 
	UE_LOG(LogFlags, Log, TEXT("%hs: name=%s old=%3.3f"),
		__func__, *Name.ToString(), Val);
	
	Flags.Remove(Name);
	OnMod.Broadcast(Name, -Val, 0);
}

void UFlags::Set(const FName& Name, float Val) {
	// this is basically duplicated code with mod...
	// But it will be faster than getting and mod'ing (for about one call to Get)
	if (Name.IsNone()) return;

	const float Old = Get(Name); // broadcasting the diff is what adds complexity here
	UE_LOG(LogFlags, Log, TEXT("Set: name=%s old=%3.3f new=%3.3f"), *Name.ToString(), Old, Val);

	Flags.Add(Name, Val);
	OnMod.Broadcast(Name, Val-Old, Val);
}

void UFlags::SetAll(const TMap<FName, float>& NewFlags) {
	UE_LOG(LogFlags, Log, TEXT("SetAll: Num=%i"), NewFlags.Num());
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
	// probably faster than calling Rem. since it removes all at once.
	UE_LOG(LogFlags, Log, TEXT("Clear: Reserve=%i"), Reserve);
	TArray<FName> Keys;
	Flags.GetKeys(Keys);
	for (const FName& K: Keys) {
		float* pV = Flags.Find(K);
		if (!pV) continue;

		// important but forgot why.
		OnMod.Broadcast(K, -*pV, 0.0);
	}

	Flags.Empty(Reserve);
}
