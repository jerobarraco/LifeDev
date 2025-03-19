// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "Behave/Test/Tasks/BFisher.h"

#include "BMove.generated.h"

class ABFish;

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBMove: public UBFisher {
	GENERATED_BODY()
public:
	UBMove();

	virtual bool Plan_Implementation() override;
	virtual void SetState_Implementation(const EBState New) override;
	virtual EBDoRes DoSelf_Implementation(const float DT) override;

	inline static FName SID = "Move";

protected:
	virtual void CostCalc_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void SetTarget();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Config)
	float CostPerDist = .1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Target = FVector::ZeroVector;
};
