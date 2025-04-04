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
	UCAnimatorTime();

	virtual void Update_Implementation(const float Alpha) override;

	// Dilation factor to start from
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float From = 1;
	// Dilation factor to go to
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float To = .1;
	// The last dilation set
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient)
	float Dilation = 1;
};
