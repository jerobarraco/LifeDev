// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "BBase.h"

#include "BFirst.generated.h"

// picks the first action that passes.
UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBFirst: public UBBase {
	GENERATED_BODY()

public:
	virtual bool Plan_Implementation() override;
	virtual void SetState_Implementation(EBState New) override;
	virtual float Cost_Implementation() override;
};
