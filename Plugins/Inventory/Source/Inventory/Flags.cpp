// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "Flags.h"

DEFINE_LOG_CATEGORY_STATIC(LogFlags, Log, Log);

UFlags* UFlags::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;
	
	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!IsValid(W))) return nullptr;

	UFlags* const I = W->GetSubsystem<UFlags>();
	return LIKELY(IsValid(I)) ? I : nullptr;
}

void UFlags::Mod(const FName& Name, const float Diff, const bool Log) {
	if (UNLIKELY(Name.IsNone())) return;
	
	const float Val = Get(Name) + Diff;
	UE_CLOG(Log, LogFlags, Log, TEXT("%hs: name=%s diff=%3.3f new=%3.3f"),
		__func__, *Name.ToString(), Diff, Val);
	
	Flags.Add(Name, Val);
	OnMod.Broadcast(Name, Diff, Val);
}

void UFlags::Rem(const FName& Name) {
	if (UNLIKELY(Name.IsNone())) return;
	
	const float Val = Get(Name); 
	UE_LOG(LogFlags, Log, TEXT("%hs: name=%s old=%3.3f"),
		__func__, *Name.ToString(), Val);
	
	Flags.Remove(Name);
	OnMod.Broadcast(Name, -Val, 0);
}

float UFlags::Get(const FName Name, const float Default) const {
	if (UNLIKELY(Name.IsNone())) return 0.0; // important so that Has return false

	const float* const PreFlag = Flags.Find(Name);
	const float Val = PreFlag ? *PreFlag : Default;
	return Val;
}

bool UFlags::IsSet(const FName& Name) const {
	if (UNLIKELY(Name.IsNone())) return false;
	// Set does not remove on 0
	return Flags.Contains(Name);
}

bool UFlags::Has(const FName& Name) const {
	const float V = Get(Name, 0);
	return FMath::IsNearlyEqual(V, 1) || V >= 1.0;
}

void UFlags::Set(const FName& Name, const float Val) {
	// this is basically duplicated code with mod...
	// But it will be faster than getting and mod'ing (for about one call to Get)
	if (UNLIKELY(Name.IsNone())) return;

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
		const float* const pV = NewFlags.Find(K);
		if (UNLIKELY(!pV)) continue;
		
		Set(K, *pV);
	}
}

void UFlags::Clear(const int32 Reserve) {
	// probably faster than calling Rem. since it removes all at once.
	UE_LOG(LogFlags, Log, TEXT("Clear: Reserve=%i"), Reserve);
	TArray<FName> Keys;
	Flags.GetKeys(Keys);
	for (const FName& K: Keys) {
		const float* const pV = Flags.Find(K);
		if (UNLIKELY(!pV)) continue;

		// important but forgot why. (notice "-")
		OnMod.Broadcast(K, -*pV, 0.0);
	}

	Flags.Empty(Reserve);
}

void UFlags::FlagsDump() {
	TArray<FName> Keys;
	Flags.GetKeys(Keys);
	for (const FName& K: Keys) {
		const float* const pV = Flags.Find(K);
		if (UNLIKELY(!pV)) continue;
		UE_LOG(LogFlags, Log, TEXT("%hs name=%s\t\t val=%.5f"), __func__, *K.ToString(), *pV);
	}
}
