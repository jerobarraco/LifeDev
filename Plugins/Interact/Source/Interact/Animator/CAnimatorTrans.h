// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "CAnimator.h"

#include "CAnimatorTrans.generated.h"

class UCurveFloat;
class USceneComponent;

// An interactive actor that can have an animation. Use AnimatorMix
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent, DeprecatedNode))
class INTERACT_API UCAnimatorTrans: public UCAnimator {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, CallInEditor)
	void CopyTStart();

	// uses accumulated (relative to start) version, or not.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	bool IsAdditive = true;

	// whether to sweep on trans updates, causing collisions.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	bool UseSweep = false;
	
	// The start transform. You need to set this.
	// This is the RelativeTransform for the TRoot.
	// On IsAdditive, this is the base of the transform (the origin).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	FTransform TStart = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	
	// The target transform. If IsAdditive, this is added onto TStart.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	FTransform TEnd = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);

	// the component to be animated (hint, use the root component for "global" positioning)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	TObjectPtr<USceneComponent> TRoot = nullptr;

protected:
	// TODO also the progress?
	virtual void Update_Implementation(const float Alpha) override;
	virtual void DeInit() override;
	virtual void BeginPlay() override;
};
