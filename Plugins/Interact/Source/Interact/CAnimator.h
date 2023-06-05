// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once

#include "CAnimator.generated.h"

class UCurveFloat;
class USceneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCAnimatorRawOnEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCAnimatorRawOnBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCAnimatorRawOnUpdate, float, Progress, float, Alpha);

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimator: public UActorComponent {
	GENERATED_BODY()
public:

	UCAnimator();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Play(bool Reversed = false, bool Loop = false, bool Bounce = false);

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Stop();

	UFUNCTION(BlueprintCallable)
	inline bool GetIsAnimating() { return IsAnimating; }
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	UCurveFloat* Curve = nullptr;

	// Duration of the animation in seconds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float Duration = 2.0;

	// triggers when the animation ends. but not if it wasn't playing.
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp")
	FCAnimatorRawOnEnd OnEnd;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp")
	FCAnimatorRawOnBegin OnBegin;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp")
	FCAnimatorRawOnUpdate OnUpdate;
	
protected:
	// override me on child classes :) (Progress can be read directly)
	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void Update(float Alpha);
	virtual void Update_Implementation(float Alpha) {};

	// override me on child classes :)
	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void End();
	virtual void End_Implementation() {};
	// override me on child classes :)
	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void Begin();
	virtual void Begin_Implementation(){};
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void SetIsAnimating(bool NewIsRotating);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsLooping = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsBouncing = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsReversed = false;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsAnimating = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	float Progress = 0.0;
};

