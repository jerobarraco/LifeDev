// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "CBehave.h"

#include "Behaves/BBase.h"
#include "Behaves/BConsts.h"
#include "Behaves/Bio/BBio.h"
#include "Behaves/Emo/BEmo.h"
#include "Behaves/Space/BSpace.h"

DEFINE_LOG_CATEGORY_STATIC(LogCBehave, Log, Log);
#pragma optimize("", off)
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
	ReactAllStates(DeltaTime);

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
	World->GetTimerManager().SetTimer(H, this, &UCBehave::Dump, 1, true);
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
	UE_LOG(LogCBehave, Log, TEXT("%hs %s DoRes=%s"),
		__func__, *GetNameSafe(this), *UEnum::GetValueAsString(DoRes));
	for (const FName& N: Plan) {
		UE_LOG(LogCBehave, Log, TEXT("%hs %s Plan=%s"),
		__func__, *GetNameSafe(this), *N.ToString());
	}
	
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue;
		B->Dump();
	}
}

void UCBehave::Do(const float DT) {
	if (Plan.IsEmpty()) {
		RePlan(); // schedule a new want. or should i wait? 
		return; // what want can fail to add a new one.
	}

	FName Want = Plan.Last();
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue;

		// TODO can be optimizing by storing the current doing in case of Do.
		// New, finish, ignore should not store the current.
		DoRes = B->Do(DT, Want); // passing want as out. don't care atm
		// if one is doing. that's it.
		// don't assume Finish, that would be problematic. (by comparing with ignore)
		// it's ok to assume ignore
		if (DoRes == EBDoRes::DO || DoRes==EBDoRes::FINISH)
			break;

		if (DoRes == EBDoRes::NEW) {
			Plan.Push(Want);
			OnDo.Broadcast(Want);
			UE_LOG(LogCBehave, Log, TEXT("%hs NewWant want=%s total=%i"),
				__func__, *Want.ToString(), Plan.Num());
			return; // have to check in again for all behaves
		}
	}

	if (DoRes == EBDoRes::IGNORE) { // if completely ignored. remove.
		if (Plan.Num()>0)
			Plan.Pop(EAllowShrinking::No);
		return;
	}

	if (DoRes == EBDoRes::FINISH) {
		UE_LOG(LogCBehave, Log, TEXT("%hs Finish want=%s"), __func__, *Want.ToString());
		if (Plan.Num()>0)
			Plan.Pop(EAllowShrinking::No);
		RePlan();
		return;
	}

	if (DoRes == EBDoRes::DO) {
		// UE_LOG(LogCBehave, Log, TEXT("%hs Do want=%s"), __func__, *Want.ToString());
		FName NewWant = Want;
		bool Break = false;
		for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
			UBBase* const B = KV.Value.Get();
			if (UNLIKELY(!IsValid(B))) continue;

			// the order ensures the execution of ReacTDo
			Break = B->ReactDo(DT, NewWant) || Break; // passing want as out. don't care atm
		}

		if (Break) { // force finish. outside for to let all components update
			UE_LOG(LogCBehave, Log, TEXT("%hs ForceBreak"), __func__);
			// this is fine. because it won't replace the current want if it's lower prio
			RePlan();
			return;
		}
	}
}

float UCBehave::TraitWantMod(const FName& Token, float V) {
	for (const TTuple<FName, FBTrait>T: Traits) {
		for (const FBTraitMod& M: T.Value.Mods ) {
			if (M.Attr != Token) continue;
			V = (V+M.Offset)*M.Factor;
		}
	}
	return V;
}

void UCBehave::RePlan() {
	// just forget the old plan.
	// that could cause issues with going to the kitchen and not wanting to eat anymore
	// but getting the replan to work well requires much more work and tweak and is prone to errors and feedbacks
	FName Want = NAME_None;
	float VMax = 0;
	FName NewWant;
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue;

		float Val = B->TopWant(NewWant);
		Val = TraitWantMod(NewWant, Val);
		if (UNLIKELY(!NewWant.IsNone() && Val>VMax)) {
			Want = NewWant;
			VMax = Val;
		}
	}

	Plan.Empty(1);
	if (Want.IsNone()) return;

	Plan.Push(Want);
	PlanVal = VMax;
	OnDo.Broadcast(Want);
	
	UE_LOG(LogCBehave, Log, TEXT("%hs %s TopWant=%s Val=%.5f"),
		__func__, *GetNameSafe(this), *Want.ToString(), PlanVal);
}

void UCBehave::ReactAllStates(const float DeltaTime) {
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue;
	
		for (const TTuple<FName, float>& KV2: B->Values) { // iterating tokens instead of values on purpose
			ReactState(DeltaTime, KV2.Key, KV2.Value);
		}
	}
}

void UCBehave::ReactState(const float DT, const FName& Name, const float V) {
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV3: Behaves) {
		UBBase* const B2 = KV3.Value.Get();
		if (UNLIKELY(!IsValid(B2))) continue; 
		B2->ReactState(DT, Name, V);
	}
}

void UCBehave::TraitMod(const FBTrait& Trait, const bool Add) {
	if (UNLIKELY(Trait.Name.IsNone())) return;
	if (Add)
		Traits.Add(Trait.Name, Trait);
	else
		Traits.Remove(Trait.Name);
}

void UCBehave::TraitAdd(const FBTrait& Trait) {
	if (UNLIKELY(Trait.Name.IsNone())) return;
	Traits.Add(Trait.Name, Trait); // Gonna copy it anyway
}

void UCBehave::TraitRem(const FName& Name) {
	Traits.Remove(Name);
}

FBTrait UCBehave::TraitGet(const FName& Name) {
	if (Traits.Contains(Name)) return Traits[Name];
	return FBTrait();
}

#pragma optimize("", on)

void UCBehave::WhatWant() {
	// i can do this once i store the want val
	// if (Plan.Num()>0) {
	// 	Want = Plan[Plan.Num()-1];
	// 	Plan.RemoveAtSwap(Plan.Num()-1, EAllowShrinking::No);
	// }

	// assume a plan is for ONE want. we can still want many things at once. handled by each behave
	if (!Plan.IsEmpty()) return;
	
	FName Want = NAME_None;
	FName NewWant;
	float VMax = 0;
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue;

		const float Val = B->TopWant(NewWant);
		if (UNLIKELY(!NewWant.IsNone() && Val>VMax+FMath::RandRange(-0.01, 0.01))) {
			Want = NewWant;
			VMax = Val;
		}
	}

	// if ((!Want.IsNone()) && (Plan.IsEmpty() || Want != Plan.Last()))
	if (!Want.IsNone()) {
		Plan.Push(Want);
		PlanVal = VMax;
	}

	UE_LOG(LogCBehave, Log, TEXT("%hs %s TopWant=%s"),
		__func__, *GetNameSafe(this), *Want.ToString());
}

void UCBehave::PlanCheck() {
	FName Want = NAME_None;
	DoRes = EBDoRes::IGNORE;
	while (true) {
		const int32 Num = Plan.Num();
		if (Num<1) break;
		Plan.Pop(EAllowShrinking::No);
		if (Num<2) break;

		Want = Plan.Last();
		float WantVal = -1;
		// check if still want it
		for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
			UBBase* const B = KV.Value.Get();
			if (UNLIKELY(!IsValid(B))) continue;

			WantVal = FMath::Max(WantVal, B->Want(Want));
		}

		if (WantVal > 0) break; // still wants it.
	}
}