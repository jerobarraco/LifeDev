// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "CBehave.h"

#include "Actions/BBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogCBehave, Log, Log);
#pragma optimize("", off)
UCBehave::UCBehave():Super() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetTickableWhenPaused(false);
	SetComponentTickEnabled(true);
}

void UCBehave::TickComponent(const float DeltaTime, const enum ELevelTick TickType,
	FActorComponentTickFunction* const ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (UNLIKELY(!ActionCur)) {
		SetComponentTickEnabled(false);
	return;
	}
	ActionCur->Do(DeltaTime);
}
#pragma optimize("", on)
