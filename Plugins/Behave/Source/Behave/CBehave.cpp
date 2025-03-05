// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "CBehave.h"

#include "Behaves/BBase.h"

UCBehave::UCBehave():Super() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetTickableWhenPaused(false);
	SetComponentTickEnabled(true);
}

void UCBehave::TickComponent(const float DeltaTime, const ELevelTick TickType,
FActorComponentTickFunction* const ThisTickFunction) {

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// update
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue; 
		B->Tick(DeltaTime);
	}

	// react
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue;
		for (const FName& T: B->Tokens) { // iterating tokens instead of values on purpose
			float* const pVal = B->Values.Find(T);
			if (UNLIKELY(!pVal)) continue;
			for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV2: Behaves) {
				UBBase* const B2 = KV2.Value.Get();
				if (UNLIKELY(!IsValid(B2))) continue; 
				B2->React(DeltaTime, T, *pVal);
			}
		}
	}
}

void UCBehave::BeginPlay() {
	Super::BeginPlay();
	
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = NewObject<UBBase>(this, KV.Key.Get());
		if (UNLIKELY(!IsValid(B))) {
			UE_LOG(LogTemp, Warning, TEXT("Behave:%hs falied to create obj for class=%s"),
				__func__, *KV.Key.Get()->GetName());
			continue;
		}

		Behaves[KV.Key] = B;
	}

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, this, &UCBehave::Dump, 2, true);
	FTimerHandle H2;
	World->GetTimerManager().SetTimer(H2, this, &UCBehave::WhatWant, 5, true);
}

void UCBehave::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue; 
		B->End();
	}

	Behaves.Empty();
	Super::EndPlay(EndPlayReason);
}

void UCBehave::Dump() {
	UE_LOG(LogTemp, Log, TEXT("%hs %s TopWant=%s"),
		__func__, *GetNameSafe(this), *TopWant.ToString());
	
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue;
		B->Dump();
	}
}

void UCBehave::WhatWant() {
	// want
	TopWant = NAME_None;
	float VMax = -1;
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue;

		FName Want;
		const float Val = B->TopWant(Want);
		if (UNLIKELY(!Want.IsNone() && Val>VMax+FMath::RandRange(-0.01, 0.01))) {
			TopWant = Want;
			VMax = Val;
		}
	}

	// TODO if the want is too strong. delay getting a new one.
	UE_LOG(LogTemp, Log, TEXT("%hs %s TopWant=%s"),
		__func__, *GetNameSafe(this), *TopWant.ToString());
}
