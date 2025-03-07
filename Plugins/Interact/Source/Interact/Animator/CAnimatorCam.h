// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once
#include "CAnimator.h"
#include "CAnimatorCam.generated.h"

// Animator for the current camera
// This only works with cameras that are not being moved while this is animating
// it will take the current camera position at the beginning of playing
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorCam: public UCAnimator {
	GENERATED_BODY()

public:
	// Target point to look at
	UPROPERTY(BlueprintReadWrite, Category="SetUp")
	FVector Target;
	
protected:
	virtual void Begin_Implementation() override;
	virtual void Update_Implementation(const float Alpha) override;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<APlayerController> Controller = nullptr;

	FRotator CamRotEnd;
	FRotator CamRotStart;
};
