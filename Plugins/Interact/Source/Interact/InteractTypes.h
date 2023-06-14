#pragma once

// Results for using items
UENUM(BlueprintType)
enum class EItemUseResult: uint8 {
	SUCCESS,

	// not a target available
	NO_TARGET,
	// the target rejected the item
	BAD_TARGET,
	// the target rejected the item AND handled it (e.g. showed a dialog)
	BAD_HANDLED
};
