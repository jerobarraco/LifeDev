// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "BTypes.generated.h"

class UBBase;

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

	STOPPING,
	ABORTING,
	MAX UMETA(HIDDEN)
};

USTRUCT(Blueprintable, BlueprintType)
struct BEHAVE_API FBAction: public FTableRowBase {
	GENERATED_BODY()

public:
	// these are Rows IDs
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> Children;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UBBase> Class;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FString Params;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FString Comment;
};