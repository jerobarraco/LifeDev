// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "BBase.h"

#include "BPick.generated.h"

// base action for a choosing actions. It will perform the least costly of its children.
UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBPick: public UBBase {
	GENERATED_BODY()
public:
	virtual EBDoRes Do_Implementation(const float DT) override;
	virtual bool Plan_Implementation() override;
	virtual bool CanDoSelf_Implementation() const override;
	virtual float CostPlan_Implementation() const override;
protected:
	UPROPERTY(Transient)
	TObjectPtr<UBBase> Chosen = nullptr;
};
