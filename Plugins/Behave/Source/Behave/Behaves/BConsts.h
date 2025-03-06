// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "BConsts.generated.h"

UENUM(Blueprintable, BlueprintType)
enum class EBDoRes : uint8 {
	IGNORE,
	DO,
	NEW,
	FINISH,
	MAX UMETA(HIDDEN)
};

USTRUCT(Blueprintable, BlueprintType)
struct FBWant {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Transient)
	FName Want = NAME_None;
	UPROPERTY(BlueprintReadWrite, Transient)
	float Val =0;
};

USTRUCT(Blueprintable, BlueprintType)
struct FBWantNorm {
	GENERATED_BODY()
public:
	// min to start wanting
	UPROPERTY(BlueprintReadWrite, Transient)
	float Thresh = 0;

	// max value to map to. val(Thresh->1) = 0->Target.
	UPROPERTY(BlueprintReadWrite, Transient)
	float Target = 0;
};