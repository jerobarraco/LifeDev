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