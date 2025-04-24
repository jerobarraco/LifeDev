// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "LearnMan.generated.h"

// for new users popups, and stuff.
UCLASS(Blueprintable, BlueprintType)
class ALearnMan: public AInfo {
	GENERATED_BODY()
public:
	ALearnMan();
	
	UFUNCTION(BlueprintCallable)
	void Init(UDataTable* const Data) { // TODO
		};
	UFUNCTION(BlueprintCallable)
	void DeInit() {};
	
};
