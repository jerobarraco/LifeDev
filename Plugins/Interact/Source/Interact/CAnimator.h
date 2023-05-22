// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once

#include "CAnimator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCAnimatorOnEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCAnimatorOnChange, float, Progress, float, Alpha);

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimator: public UActorComponent {
	GENERATED_BODY()
public:

	UCAnimator();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Play(bool Reversed = false);

	UFUNCTION(BlueprintCallable)
	inline bool GetIsAnimating() { return IsAnimating; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	UCurveFloat* Curve = nullptr;

	// Duration of the animation in seconds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float Duration = 2.0;

	// uses accumulated or non accumulated version. accumulated will replace the start transform with the one at beginplay
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	bool IsAccumulated = false;
	
	// The animation transform
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FTransform TStart = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);
	
	// The animation transform
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	FTransform TEnd = FTransform(FRotator::ZeroRotator, FVector::ZeroVector, FVector::OneVector);

	// the component to be animated (hint, use the root component for "global" positioning)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	USceneComponent* AnimRoot = nullptr;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp")
	FCAnimatorOnEnd OnEnd;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp")
	FCAnimatorOnChange OnChange;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void SetIsAnimating(bool NewIsRotating);
	// starts closed
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsAnimating = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	float Progress = 0.0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsReversed = false;
};
