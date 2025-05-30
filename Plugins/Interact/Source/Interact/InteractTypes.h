// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

// Results for using items
UENUM(BlueprintType)
enum class EItemUseResult: uint8 {
	// all good, item used
	SUCCESS,
	// internal error
	ERROR,
	// the target rejected the item AND handled it (e.g. showed a dialog)
	BAD_HANDLED,
	MAX UMETA(HIDDEN),
};
ENUM_RANGE_BY_COUNT(EItemUseResult, EItemUseResult::MAX);