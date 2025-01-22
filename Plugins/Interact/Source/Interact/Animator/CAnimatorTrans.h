// Copyright (C) 2023 - Jerónimo Barraco-Mármol

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
	// uses accumulated (relative to start) version, or not.
	// When "IsAdditive" it is going to set "TStart" automatically to the current transform of the AnimRoot on begin play.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	bool IsAdditive = true;

	// whether to sweep on trans updates, causing collisions.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	bool UseSweep = false;
	
	// The animation transform. You don't necessarily need to set this up, but you can change it.
	// When "IsAdditive" it is going to be automatically set to the current transform of the AnimRoot on begin play.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	FTransform TStart = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);
	
	// The animation transform
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	FTransform TEnd = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);

	// the component to be animated (hint, use the root component for "global" positioning)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	TObjectPtr<USceneComponent> TRoot = nullptr;

protected:
	virtual void Update_Implementation(float Alpha) override;
	virtual void DeInit() override;
	virtual void BeginPlay() override;
};
