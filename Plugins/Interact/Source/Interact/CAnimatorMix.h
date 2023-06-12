// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "CAnimator.h"

#include "CAnimatorMix.generated.h"

class UCurveFloat;
class USceneComponent;

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorMix: public UCAnimator {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void DeInit();

	///// Transform
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

	// ~Transform
	// Mats
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FName MatFName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	float MatFStart = 0.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	float MatFEnd = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FName MatVName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FVector MatVStart;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	FVector MatVEnd = FVector::OneVector;

	// the component to be animated (hint, use the root component for "global" positioning)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Material")
	UMaterialInstanceDynamic* Mat = nullptr;
	
protected:
	virtual void Update_Implementation(float Alpha) override;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
