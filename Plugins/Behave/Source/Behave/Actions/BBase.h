// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once


#include "BBase.generated.h"

// base for action behaviors. the idea is that you make your own and each one handles what it needs.
// a top level action, is considered a goal.
UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBBase: public UObject {
	GENERATED_BODY()

public:

};
