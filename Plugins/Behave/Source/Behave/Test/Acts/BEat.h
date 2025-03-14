// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "Behave/Actions/BBase.h"

#include "BEat.generated.h"

class ABFish;

UCLASS(Blueprintable, BlueprintType, Config=Behave, DefaultConfig)
class BEHAVE_API UBEat: public UBBase {
	GENERATED_BODY()
public:
	UBEat();

	virtual bool Plan_Implementation() override;
	
	virtual void SetState_Implementation(const EBState New) override;
	virtual EBDoRes Do_Implementation(const float DT) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Target = FVector::ZeroVector;

	inline static FName SID = "Eat";

protected:
	virtual void Register_Implementation(UCBehave* const B) override;

	UPROPERTY(Transient)
	TObjectPtr<ABFish> Fish = nullptr;
};
