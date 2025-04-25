// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "LearnMan.h"

#include "LearnTypes.h"
#include "Inventory/Flags.h"

DEFINE_LOG_CATEGORY_STATIC(LogLearnMan, Log, Log)

namespace Inventory { namespace Learn {
	static const TCHAR * const Prefix = TEXT("Learn.");
}}


ALearnMan::ALearnMan():Super() {
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void ALearnMan::Init_Implementation(UDataTable* const Data) {
	UE_LOG(LogLearnMan, Log, TEXT("%hs DT=%s"), __func__, *GetNameSafe(Data));
	DT = Data;
	Flags = UFlags::Instance(this);
	UE_CLOG(!Flags, LogLearnMan, Warning, TEXT("%hs Flag subsystem not found!"), __func__);
}

void ALearnMan::DeInit_Implementation() {
	DT = nullptr;
	Flags = nullptr;
}

bool ALearnMan::Show(const FName& Id) {
	UE_LOG(LogLearnMan, Log, TEXT("%hs Id=%s"), __func__, *Id.ToString());
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!DT | !Flags | !W | !CurrentId.IsNone())) {
		UE_LOG(LogLearnMan, Warning, TEXT("%hs DT or Flags or World is not ok Or Busy. DT=%s"), __func__, *GetNameSafe(DT));
		return false;
	}

	const FLearnRow* const pR = DT->FindRow<FLearnRow>(Id, "");
	if (UNLIKELY(!pR)) {
		UE_LOG(LogLearnMan, Log, TEXT("%hs Row not found. Row=%s"), __func__, *Id.ToString());
		return false;
	}

	CurrentId = Id;
	const FName FN(Inventory::Learn::Prefix + Id.ToString());
	if (UNLIKELY(Flags->Has(FN))) {
		UE_LOG(LogLearnMan, Log, TEXT("%hs User already saw this. Row=%s"), __func__, *Id.ToString());
		return true; // Todo false or true?
	}

	OnShow.Broadcast(Id, *pR);
	FTimerHandle H;
	// const bool T = (pR->Time) >0 ? (pR->Time) : Time; // TODO figure why this doesn't work
	W->GetTimerManager().SetTimer(H, this, &ALearnMan::HideCurrent, Time);
	return true;
}

void ALearnMan::Hide(const FName& Id) {
	UE_LOG(LogLearnMan, Log, TEXT("%hs Id=%s"), __func__, *Id.ToString());
	if (UNLIKELY(Id.IsNone() | !Flags)) {
		UE_LOG(LogLearnMan, Warning, TEXT("%hs I have nothing to hide. Or no flags."), __func__);
		return;
	}

	// allow to pre-emptively mark actions as learnt
	const FName FN(Inventory::Learn::Prefix + Id.ToString());
	Flags->Set(FN);

	// and also hide the current one
	if (UNLIKELY(Id != CurrentId)) return;

	CurrentId = NAME_None;
	
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	World->GetTimerManager().ClearAllTimersForObject(this); // in case it's called from somewhere else

	OnHide.Broadcast();
}
