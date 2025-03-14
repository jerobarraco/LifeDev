// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "CBehave.h"

DEFINE_LOG_CATEGORY_STATIC(LogCBehave, Log, Log);
#pragma optimize("", off)
UCBehave::UCBehave():Super() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetTickableWhenPaused(false);
	SetComponentTickEnabled(true);
}
#pragma optimize("", on)
