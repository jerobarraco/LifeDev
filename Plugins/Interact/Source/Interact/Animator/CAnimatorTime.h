// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once
#include "CAnimator.h"
#include "CAnimatorTime.generated.h"

// Animator for the time dilation
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorTime: public UCAnimator {
	GENERATED_BODY()

public:
	virtual void Update_Implementation(const float Alpha) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float From = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float To = 1;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient)
	float Dilation = 1;
};
