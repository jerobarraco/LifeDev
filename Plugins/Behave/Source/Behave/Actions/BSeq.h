// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "BBase.h"

#include "BSeq.generated.h"

// base action for a sequence action. it will perform it's children in sequence.
UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBSeq: public UBBase {
	GENERATED_BODY()
public:
	virtual EBDoRes DoSelf_Implementation(const float DT) override;
	virtual bool Plan_Implementation() override;
	virtual void SetState_Implementation(const EBState New) override;
protected:
};
