// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "BBase.h"

#include "BMulti.generated.h"

// base action for parallel actions. it will perform it's children in parallel. stopping when all stopped.
UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBMulti: public UBBase {
	GENERATED_BODY()

public:
	virtual EBDoRes DoSelf_Implementation(const float DT) override;
	virtual bool Plan_Implementation() override;
	virtual void SetState_Implementation(const EBState New) override;
	virtual void CostCalc_Implementation() override;
};
