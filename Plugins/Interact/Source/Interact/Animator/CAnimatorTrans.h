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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	bool IsAdditive = true;
	
	// The animation transform. You don't necessarily need to set this up, but you can change it.
	// It's going to be automatically set to the current transform of the AnimRoot
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform TStart = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
	
	// The animation transform
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	FTransform TEnd = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::ZeroVector);

	// the component to be animated (hint, use the root component for "global" positioning)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Transform")
	USceneComponent* TRoot = nullptr;

protected:
	virtual void Update_Implementation(float Alpha) override;
	virtual void DeInit() override;
	virtual void BeginPlay() override;
};
