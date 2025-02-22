// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "CoreMinimal.h"

#include "EvalMath.generated.h"

// Subsystem that evaluates math expressions
UCLASS(Blueprintable, Category="JUtils", Config=JUtils, DefaultConfig)
class JUTILS_API UEvalMath: public UWorldSubsystem {
	GENERATED_BODY()

public:

#pragma region Base
	static UEvalMath* Instance(const UObject* const O);

	UEvalMath();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
#pragma endregion

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config, Category=JUtilsMath)
	bool UseSubsystem = true;
};
