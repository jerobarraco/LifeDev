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
struct FBWantNorm {
	GENERATED_BODY()
public:
	// min to start wanting
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Thresh = 0;

	// max value to map to. val(Thresh->1) = 0->Target.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Target = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Error = .05;
};