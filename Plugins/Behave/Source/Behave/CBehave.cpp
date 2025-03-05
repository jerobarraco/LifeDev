// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "CBehave.h"

#include "Behaves/BBase.h"
#include "Behaves/BConsts.h"
#include "Behaves/Bio/BBio.h"
#include "Behaves/Emo/BEmo.h"
#include "Behaves/Space/BSpace.h"

DEFINE_LOG_CATEGORY_STATIC(LogCBehave, Log, Log);

UCBehave::UCBehave():Super() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetTickableWhenPaused(false);
	SetComponentTickEnabled(true);
	Behaves = {
		{ UBBio::StaticClass(), nullptr },
		{ UBEmo::StaticClass(), nullptr },
		{ UBSpace::StaticClass(), nullptr },
	};
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
				B2->ReactState(DeltaTime, T, *pVal);
			}
		}
		// TODO react to Want not now because it's surely broken
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

	if (Want == UBBio::T_Hungry && (Plan.IsEmpty() || Want != Plan[Plan.Num()-1])) // forcing hungry to test
		Plan.Push(Want);

	// TODO if the want is too strong. delay getting a new one.
	UE_LOG(LogCBehave, Log, TEXT("%hs %s TopWant=%s"),
		__func__, *GetNameSafe(this), *Want.ToString());
}

void UCBehave::Do(const float DT) {
	if (Plan.IsEmpty()) {
		WhatWant(); // schedule a new want. or should i wait? // TODO wait and let want arise normally. have a period of satisfaction.
		return; // what want can fail to add a new one.
	}

	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue;

		DoRes = B->Do(DT, Want); // passing want as out. don't care atm
		// if one is doing. that's it. TODO enable concurrent actions
		if (DoRes == EBDoRes::DO) break;

		// don't assume Finish, that would be problematic. (by comparing with ignore)
		// it's ok to assume ignore
		if (DoRes == EBDoRes::FINISH) break;

		if (DoRes == EBDoRes::NEW) {
			Plan.Push(Want);
			UE_LOG(LogCBehave, Log, TEXT("%hs NewWant want=%s total=%i"),
				__func__, *Want.ToString(), Plan.Num());
			return; // have to check in again for all behaves
		}

		// return; // need to start all over
	}
	 
	if (DoRes == EBDoRes::DO) {
		for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
			UBBase* const B = KV.Value.Get();
			if (UNLIKELY(!IsValid(B))) continue;
			
			B->ReactDo(DT, Want); // passing want as out. don't care atm
		}
	} else if (DoRes == EBDoRes::FINISH) {
		UE_LOG(LogCBehave, Log, TEXT("%hs Finish want=%s"), __func__, *Want.ToString());
		// TODO fix this part is not working
		Want = NAME_None;
		while (Plan.Num()>0) {
			const int32 Num = Plan.Num();
			Plan.RemoveAtSwap(Num-1, EAllowShrinking::No);
			if (Num<2) break;

			Want = Plan[Num-2]; // uops 2
			float WantVal = -1;
			// check if still want it
			for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
				UBBase* const B = KV.Value.Get();
				if (UNLIKELY(!IsValid(B))) continue;
				WantVal = FMath::Max(WantVal, B->Want(Want));
			}
			if (WantVal > .15) break; // still wants it.
		}
	}
}
