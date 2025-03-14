// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "BTypes.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EBDoRes : uint8 {
	CONTINUE,
	STOP,
	ABORT,
	MAX UMETA(HIDDEN)
};

UENUM(Blueprintable, BlueprintType)
enum class EBState : uint8 {
	STOPPED,
	STARTED,
	MAX UMETA(HIDDEN)
};
