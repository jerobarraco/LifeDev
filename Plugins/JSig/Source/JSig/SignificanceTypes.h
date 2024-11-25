// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CoreMinimal.h"
#include "SignificanceTypes.generated.h"

// Defines the different levels of significance.
// Off means no significance at all.
// some things should/could/would be disabled here 
UENUM(BlueprintType, Blueprintable)
enum class ESigValue : uint8 {
	// Disable everything here
	Off = 0,
	Low,
	Med,
	// No stripping/culling
	High,
	MAX UMETA(Hidden)
};
