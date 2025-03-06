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

// this sucks a bit. i'm not sure if a trait should modify the want or should modify the behavior
USTRUCT(Blueprintable, BlueprintType)
struct FBTraitMod {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Attr;

	// added to the want
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Offset;

	// multiplies the want. offset goes first
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Factor;
};

USTRUCT(Blueprintable, BlueprintType)
struct FBTrait {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FBTraitMod> Mods;
};
