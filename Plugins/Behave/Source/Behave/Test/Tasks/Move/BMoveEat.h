// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "BMove.h"

#include "BMoveEat.generated.h"


// this is stub until i make this a bit more data driven

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBMoveEat: public UBMove {
	GENERATED_BODY()
public:

	virtual bool Plan_Implementation() override;

protected:
	virtual void SetTarget_Implementation() override;
};
