// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CAnimator.h"

#include "CAnimatorMat.generated.h"

class USceneComponent;
// TODO delete

// An interactive actor that can have an animation. Derp-ecated Use UCAnimatorMix.
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent, DeprecatedNode))
class INTERACT_API UCAnimatorMat: public UCAnimator {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FName ParamFName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float FStart = 0.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float FEnd = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FName ParamVName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FLinearColor VStart = FLinearColor::Black;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FLinearColor VEnd = FLinearColor::White;

	// the component to be animated (hint, use the root component for "global" positioning)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	UMaterialInstanceDynamic* Mat = nullptr;

protected:
	virtual void Update_Implementation(float Alpha) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
