// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "Behave/Tasks/BBase.h"

#include "BFisher.generated.h"

class ABFish;

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBFisher: public UBBase {
	GENERATED_BODY()

protected:
	virtual void Init_Implementation(UCBehave* const B) override;

	UPROPERTY(Transient)
	TObjectPtr<ABFish> Fish = nullptr;
};
