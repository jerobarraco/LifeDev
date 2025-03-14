// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "Behave/Actions/BBase.h"

#include "BSleep.generated.h"

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBSleep: public UBBase {
	GENERATED_BODY()
public:
	UBSleep();

	virtual bool Plan_Implementation() override;
	
	virtual void SetState_Implementation(const EBState New) override;
	virtual EBDoRes Do_Implementation(const float DT) override;

	inline static FName SID = "Sleep";
protected:
	UPROPERTY(VisibleAnywhere, Transient)
	float TimeCur = 0;
	UPROPERTY(VisibleAnywhere, Transient)
	float TimeSleep = 2;
};
