// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "Behave/Test/Acts/BFisher.h"

#include "BMoveEat.generated.h"


UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBMoveEat: public UBFisher {
	GENERATED_BODY()
public:
	UBMoveEat();

	virtual bool Plan_Implementation() override;
	
	virtual void SetState_Implementation(const EBState New) override;
	virtual EBDoRes Do_Implementation(const float DT) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Target = FVector::ZeroVector;

	inline static FName SID = "MoveEat";
protected:
};
