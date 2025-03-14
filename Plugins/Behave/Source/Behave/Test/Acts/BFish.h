// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "Behave/Actions/BBase.h"

#include "BFish.generated.h"

class ABFish;

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBFish: public UBBase {
	GENERATED_BODY()
protected:
	virtual void Register_Implementation(UCBehave* const B) override;

	UPROPERTY(Transient)
	TObjectPtr<ABFish> Fish = nullptr;
};
