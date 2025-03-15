// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "BFirst.h"

#include "BRnd.generated.h"

// picks one. at random. like first but random.
UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBRnd: public UBFirst {
	GENERATED_BODY()

public:
	virtual bool Plan_Implementation() override;
};
