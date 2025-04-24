// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once
#include "Engine/DataTable.h"

#include "LearnTypes.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct INVENTORY_API FLearnRow: public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Time = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine))
	FText Text;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine))
	FString Comment;
};
