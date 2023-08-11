// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once

#include "CAnimator.generated.h"

class UCurveFloat;
class USceneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCAnimatorRawOnEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCAnimatorRawOnBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCAnimatorRawOnUpdate, float, Progress, float, Alpha);

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(float, FCodeCurve, float, Progress);

// An interactive actor that can have an animation
// You can set the tick interval to control the performance of this component
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimator: public UActorComponent {
	GENERATED_BODY()
public:

	UCAnimator();
	// todo fix all the bp usages of this function
	
	// play as is. mostly for delegates and play as set in defaults or when you only need to change one of the variables.
	UFUNCTION(BlueprintCallable, CallInEditor)
	inline void Play() { SetIsAnimating(true); };

	// mostly for bps when you wanna set and play at the same time. will override all 3 variables.
	UFUNCTION(BlueprintCallable, CallInEditor)
	void PlaySet(bool Reversed = false, bool Loop = false, bool Bounce = false);

	UFUNCTION(BlueprintCallable, CallInEditor)
	inline void Stop() { SetIsAnimating(false); };

	UFUNCTION(BlueprintCallable)
	inline bool GetIsAnimating() const { return IsAnimating; }

	UFUNCTION(BlueprintCallable)
	inline bool GetProgress() const { return Progress; }
	
	// a tick function for when you need to use this class somewhere else. it's hacky. yes.
	UFUNCTION(BlueprintCallable)
	void DoTick(float DeltaSeconds);

	// By default run at 30 fps; Optimization
	inline static float IntervalDefault = 1.0f/30.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	UCurveFloat* Curve = nullptr;

	// Duration of the animation in seconds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	float Duration = 2.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	bool IsLooping = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	bool IsBouncing = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	bool IsReversed = false;

	// triggers when the animation ends. but not if it wasn't playing.
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Signals")
	FCAnimatorRawOnEnd OnEnd;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Signals")
	FCAnimatorRawOnBegin OnBegin;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Signals")
	FCAnimatorRawOnUpdate OnUpdate;

	UPROPERTY(BlueprintReadWrite)
	FCodeCurve CodeCurve;


protected:
	// override me on child classes :) But call the parent. (Progress can be read directly)
	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void Update(float Alpha);
	virtual void Update_Implementation(float Alpha) {};

	// override me on child classes :) But call the parent. 
	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void End();
	virtual void End_Implementation();

	// override me on child classes :) But call the parent. 
	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void Begin();
	virtual void Begin_Implementation();
	
	UFUNCTION(BlueprintCallable)
	void SetIsAnimating(bool NewIsRotating);
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void DeInit();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsAnimating = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	float Progress = 0.0;
};
