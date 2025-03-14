// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "Behave/Actions/BBase.h"

#include "BMove.generated.h"

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBMove: public UBBase {
	GENERATED_BODY()
public:
	UBMove();

	virtual bool Plan_Implementation() override;
	
	virtual void SetState_Implementation(const EBState New) override;
	virtual EBDoRes Do_Implementation(const float DT) override;

	inline static FName SID = "Move";
protected:
};
