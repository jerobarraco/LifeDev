// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once
#include "Engine/DataTable.h"

#include "TeachTypes.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct INVENTORY_API FTeachRow: public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Time = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine))
	FText Text;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine))
	FString Comment;
};

// todo create a input type or use one from ue. and use that instead.
// probably EHardwareDevicePrimaryType or EInputDevices
UENUM(Blueprintable, BlueprintType, meta=(Deprecated))
enum class ETeachTarget: uint8 {
	NONE,
	// desktop kb and mouse
	DESK,
	// gamepad
	PAD,
	// Touchscreen (android?)
	TOUCH,
	VR,
	MAX UMETA(Hidden),
};

ENUM_RANGE_BY_COUNT(ETeachTarget, ETeachTarget::MAX);