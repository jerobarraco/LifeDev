// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "CBehave.h"

#include "Behaves/BBase.h"
#include "Behaves/BConsts.h"

DEFINE_LOG_CATEGORY_STATIC(LogCBehave, Log, Log);

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

	// do
	Do(DeltaTime);
}

void UCBehave::BeginPlay() {
	Super::BeginPlay();

	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = NewObject<UBBase>(this, KV.Key.Get());
		if (UNLIKELY(!IsValid(B))) {
			UE_LOG(LogCBehave, Warning, TEXT("Behave:%hs falied to create obj for class=%s"),
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
	UE_LOG(LogCBehave, Log, TEXT("%hs %s TopWant=%s DoRes=%s"),
		__func__, *GetNameSafe(this), *Want.ToString(), *UEnum::GetValueAsString(DoRes));
	
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue;
		B->Dump();
	}
}

void UCBehave::WhatWant() {
	// want
	Want = NAME_None;
	float VMax = -1;
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue;

		FName CurWant;
		const float Val = B->TopWant(CurWant);
		if (UNLIKELY(!CurWant.IsNone() && Val>VMax+FMath::RandRange(-0.01, 0.01))) {
			Want = CurWant;
			VMax = Val;
		}
	}

	if (Plan.IsEmpty() || Want != Plan[Plan.Num()-1])
		Plan.Push(Want);

	// TODO if the want is too strong. delay getting a new one.
	UE_LOG(LogCBehave, Log, TEXT("%hs %s TopWant=%s"),
		__func__, *GetNameSafe(this), *Want.ToString());
}

void UCBehave::Do(const float DT) {
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue;

		DoRes = B->Do(DT, Want); // passing want as out. don't care atm
		// if one is doing. that's it. TODO enable multiple actions
		if (DoRes == EBDoRes::DO) return;
		if (DoRes == EBDoRes::NEW) {
			Plan.Push(Want);
			UE_LOG(LogCBehave, Log, TEXT("%hs NewWant want=%s total=%i"),
				__func__, *Want.ToString(), Plan.Num());
			return; // have to check in again for all behaves
		}

		// don't assume Finish, that would be problematic. (by comparing with ignore)
		// it's ok to assume ignore
		if (DoRes != EBDoRes::FINISH) continue;

		UE_LOG(LogCBehave, Log, TEXT("%hs Finish want=%s"), __func__, *Want.ToString());
		const int32 Num = Plan.Num();
		if (Num < 2) { // one of them is going to be popped, and we need one more.
			Plan.Empty(1);
			WhatWant(); // schedule a new want. or should i wait? // TODO wait and let want arise normally. have a period of satisfaction.
			return;
		}

		Plan.RemoveAtSwap(Num-1, EAllowShrinking::No);
		Want = Plan[Num-1];
		return; // need to start all over
	}
}
