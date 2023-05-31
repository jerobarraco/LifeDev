// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "Step.h"
#include "Camera/CameraComponent.h"

#include "StepCam.generated.h"

class ACameraActor;

UCLASS(Blueprintable, BlueprintType)
class STORY_API AStepCam : public AStep {
	GENERATED_BODY()

public:
	AStepCam();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	USceneComponent* Root = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UCameraComponent* Cam = nullptr;
};
