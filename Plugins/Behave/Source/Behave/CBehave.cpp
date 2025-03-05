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

void UCBehave::TickComponent(const float DeltaTime, const enum ELevelTick TickType,
FActorComponentTickFunction* const ThisTickFunction) {

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	for (TTuple<TSubclassOf<UBBase>, TObjectPtr<UBBase>>KV: Behaves) {
		UBBase* const B = KV.Value.Get();
		if (UNLIKELY(!IsValid(B))) continue; 
		B->Tick(DeltaTime);
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
