// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "BFisher.h"

#include "BPlay.generated.h"

class ABFish;

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBPlay: public UBFisher {
	GENERATED_BODY()
public:
	UBPlay();

	virtual bool Plan_Implementation() override;
	virtual EBDoRes DoSelf_Implementation(const float DT) override;

	inline static FName SID = "Play";
protected:
};
