// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "BBase.h"

#include "BWait.generated.h"

// Base task for a delay
UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBWait: public UBBase {
	GENERATED_BODY()

public:
	virtual bool Plan_Implementation() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TimeWait = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config)
	float CostPerSecond = 10;

protected:
	virtual EBDoRes DoSelf_Implementation(const float DT) override;
	virtual void SetState_Implementation(const EBState New) override;
	virtual void CostCalc_Implementation() override;
	float TimeRem = 0;
};
