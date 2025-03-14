// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "BFish.h"

#include "BEat.generated.h"

class ABFish;

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBEat: public UBFish {
	GENERATED_BODY()
public:
	UBEat();

	virtual bool Plan_Implementation() override;
	virtual void SetState_Implementation(const EBState New) override;
	virtual EBDoRes Do_Implementation(const float DT) override;
	inline static FName SID = "Eat";
protected:
};
