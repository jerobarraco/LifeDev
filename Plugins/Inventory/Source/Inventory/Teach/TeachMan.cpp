// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "TeachMan.h"

#include "TeachTypes.h"
#include "Inventory/Flags.h"

DEFINE_LOG_CATEGORY_STATIC(LogTeachMan, Log, Log)

namespace Inventory { namespace Teach {
	static const TCHAR* const Prefix = TEXT("Teach.");
}}

ATeachMan::ATeachMan():Super() {
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void ATeachMan::Init_Implementation() {
	UE_LOG(LogTeachMan, Log, TEXT("%hs"), __func__);
	Flags = UFlags::Instance(this);
	UE_CLOG(!Flags, LogTeachMan, Warning, TEXT("%hs Flag subsystem not found!"), __func__);
}

void ATeachMan::DeInit_Implementation() {
	DT = nullptr;
	DTs.Empty();
	Flags = nullptr;

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	World->GetTimerManager().ClearAllTimersForObject(this);
}

bool ATeachMan::Has(const FName& Id) const {
	const FName FN(Inventory::Teach::Prefix + Id.ToString());
	return LIKELY(Flags) && Flags->Has(FN);
}

void ATeachMan::Set_Implementation(const FName& Id) {
	UE_LOG(LogTeachMan, Log, TEXT("%hs Id=%s"), __func__, *Id.ToString());
	const FName FN(Inventory::Teach::Prefix + Id.ToString());
	if (LIKELY(Flags)) Flags->Set(FN);
}

bool ATeachMan::Show_Implementation(const FName& Id) {
	UE_LOG(LogTeachMan, Log, TEXT("%hs Id=%s"), __func__, *Id.ToString());
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!CurrentId.IsNone())) {
		UE_LOG(LogTeachMan, Log, TEXT("%hs Busy."), __func__);
		return false;
	}

	if (UNLIKELY(!DT | !Flags | !W)) {
		UE_LOG(LogTeachMan, Warning, TEXT("%hs DT, Flags, or World is not ok. DT=%s"), __func__, *GetNameSafe(DT));
		return false;
	}

	if (UNLIKELY(Has(Id))) {
		UE_LOG(LogTeachMan, Log, TEXT("%hs User already saw this. Row=%s"), __func__, *Id.ToString());
		return true; // true since it's already shown.
	}

	const FTeachRow* const pR = DT->FindRow<FTeachRow>(Id, "", false);
	if (UNLIKELY(!pR)) {
		UE_LOG(LogTeachMan, Log, TEXT("%hs Row not found. Row=%s"), __func__, *Id.ToString());
		return false;
	}

	Set(Id); // mark here as well to make logic easier.
	CurrentId = Id;
	LastTime = W->GetTimeSeconds();
	UE_LOG(LogTeachMan, Log, TEXT("%hs Time =%.3f"), __func__, pR->Time);
	OnShow.Broadcast(Id, *pR);

	const float T = (pR->Time) >0 ? (pR->Time) : Time;
	FTimerManager& Timer = W->GetTimerManager();
	Timer.ClearTimer(HShow);
	HShow.Invalidate();
	Timer.SetTimer(HShow, this, &ATeachMan::HideCurrent, T);
	return true;
}

void ATeachMan::Hide_Implementation(const FName Id) { // don't use ref here.
	// don't use ref, since we return it on OnHide. there's a slight chance it's a ref to CurrentId if called with HideCurrent (on timeout).

	UE_LOG(LogTeachMan, Log, TEXT("%hs Id=%s"), __func__, *Id.ToString());
	if (UNLIKELY(Id.IsNone() | !Flags)) {
		UE_LOG(LogTeachMan, Warning, TEXT("%hs I have nothing to hide. Or no flags."), __func__);
		return;
	}

	Set(Id); // allow to pre-emptively mark actions as learnt
	// and also hide the current one
	if (UNLIKELY(Id != CurrentId)) return;

	CurrentId = NAME_None;

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	World->GetTimerManager().ClearTimer(HShow); // in case it's called from somewhere else
	HShow.Invalidate();

	OnHide.Broadcast(Id);
}

void ATeachMan::AddTarget(const ETeachTarget Tgt, UDataTable* const InDT) {
	DTs.Add(Tgt, InDT);
	if (UNLIKELY(!DT)) DT = InDT;
}

void ATeachMan::SetTarget(const ETeachTarget Tgt) {
	TObjectPtr<UDataTable>* Ptr = DTs.Find(Tgt);
	DT = Ptr ? *Ptr : nullptr;
}
