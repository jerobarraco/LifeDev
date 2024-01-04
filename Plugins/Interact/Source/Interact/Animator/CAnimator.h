// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "JUtils/Math/CodeCurve.h"

#include "CAnimator.generated.h"

class UAnimTrackBase;
class UCurveFloat;
class USceneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCAnimatorRawOnEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCAnimatorRawOnBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCAnimatorRawOnUpdate, float, Progress, float, Alpha);
	

// An interactive actor that can have an animation
// You can set the tick interval to control the performance of this component
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimator: public UActorComponent {
	GENERATED_BODY()

public:
	UCAnimator();

	// plays the animation. optionally reset.
	virtual void Activate(bool bReset) override;
	// stops the animation.
	virtual void Deactivate() override;
	
	// play as is. mostly for delegates and play as set in defaults or when you only need to change one of the variables.
	UFUNCTION(BlueprintCallable, CallInEditor, meta=(DeprecatedFunction))
	FORCEINLINE void Play() { Activate(true); }

	// mostly for bps when you wanna set and play at the same time. will override all 3 variables.
	UFUNCTION(BlueprintCallable, CallInEditor)
	void PlaySet(bool Reversed = false, bool Loop = false, bool Bounce = false);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetProgress() const { return Progress; }
	
	// a tick function for when you need to use this class somewhere else.
	UFUNCTION(BlueprintCallable)
	void TickManual(float DeltaSeconds);

	// sets/clear a parent that drives the update of this animator
	UFUNCTION(BlueprintCallable)
	void BindTo(UCAnimator* Parent = nullptr);
	
	// By default run at 30 fps; Optimization
	inline static float IntervalDefault = 1.0f/30.f;
	// to be set by game manager depending on the flags
	inline static bool Debug = false;
	
	// curve to use, if not set then codecurve will be used, if not set then it will be linear.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	UCurveFloat* Curve = nullptr;
	// a curve by code. only used if Curve is not set. if both not set it will be linear.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Setup")
	FCodeCurve CodeCurve;

	// Duration of the animation in seconds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	float Duration = 2.0;
	// can be changed directly, but beware if doing so while playing
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	bool IsLooping = false;
	// Will restart the animation once completed. If not looping, then only once. can be changed directly, but beware if doing so while playing
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	bool IsBouncing = false;
	// Whether currently reversed. changes on bounce. can be changed directly, but beware if doing so while playing
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	bool IsReversed = false;

	// triggers when the animation ends. but not if it wasn't playing.
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Transient, Category="SetUp|Signals")
	FCAnimatorRawOnEnd OnEnd;
	// when it starts, obviously
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Transient, Category="SetUp|Signals")
	FCAnimatorRawOnBegin OnBegin;
	// everytime the animation updates
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Transient, Category="SetUp|Signals")
	FCAnimatorRawOnUpdate OnUpdate;

protected:
	// override me on child classes :) But call the parent!! (Progress can be read directly)
	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void Update(float Alpha);
	virtual void Update_Implementation(float Alpha);

	// Called when the animation ends. override me on child classes :) But call the parent. 
	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void End();
	virtual void End_Implementation();

	// override me on child classes :) But call the parent. 
	UFUNCTION(BlueprintNativeEvent, Category=SetUp)
	void Begin();
	virtual void Begin_Implementation();

	// called by a parent component's update
	UFUNCTION() // bound
	void ChildUpdate(float T, float Alpha);

	void DoTick(float DeltaSeconds);
	// finishes, and tentatively restart if needed.
	void Finish();
	virtual void DeInit();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	float Progress = 0.0;
	UPROPERTY(BlueprintReadOnly, Transient)
	UCAnimator* Parent = nullptr;
	
	float DTAcum = 0.0; // used for manual ticks 
};
