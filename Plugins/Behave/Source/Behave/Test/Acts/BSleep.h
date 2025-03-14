// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "BFisher.h"
#include "Behave/Actions/BBase.h"

#include "BSleep.generated.h"

class ABFish;

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBSleep: public UBFisher {
	GENERATED_BODY()
public:
	UBSleep();

	virtual bool Plan_Implementation() override;
	virtual EBDoRes Do_Implementation(const float DT) override;

	inline static FName SID = "Sleep";
protected:
};
