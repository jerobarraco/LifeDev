// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "JUtils/Math/CodeCurve.h"

#include "CAnimator.generated.h"

class UAnimTrackBase;
class UCurveFloat;
class USceneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCAnimatorRawOnEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCAnimatorRawOnBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCAnimatorRawOnUpdate, const float, Progress, const float, Alpha);

// An interactive actor that can have an animation
// You can set the tick interval to control the performance of this component.
// Not active by default.
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimator: public UActorComponent {
	GENERATED_BODY()

public:
	UCAnimator();

	// plays the animation. optionally reset.
	// it won't end the animation until next tick. even with duration 0.
	virtual void Activate(const bool bReset=false) override;
	// stops the animation.
	virtual void Deactivate() override;

	// mostly for bps when you want to set and play at the same time. will override all 3 variables.
	UFUNCTION(BlueprintCallable, CallInEditor)
	void PlaySet(const bool Reversed = false, const bool Loop = false, const bool Bounce = false);
	// mostly for bps and binds. when you want to flip and play.
	UFUNCTION(BlueprintCallable, CallInEditor)
	FORCEINLINE void PlayMirror() {
		Mirror();
		Activate(false); // important not to reset on mirror.
	}
	// simply flips "IsReversed" to the opposite.
	// the progress remains the same, unless calling Activate(true).
	// mostly for binds
	UFUNCTION(BlueprintCallable, CallInEditor)
	FORCEINLINE void Flip() { IsReversed = !IsReversed; }
	// Inverts the direction while also inverting the progress.
	// Good for direction change while animating.
	// Not warrantied to work after the animation finishes. (but it does atm).
	// Mostly for binds.
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Mirror() {
		Flip();
		Progress = FMath::Clamp(1-Progress, 0, 1);
	}

	UFUNCTION(BlueprintCallable, CallInEditor)
	FORCEINLINE float GetProgress() const { return Progress; }

	// Override this to actually set the effect that you implement.
	// Be careful as this can be called in the constructor of the owner. (see InteractAnim::SetNow)
	// override me on child classes :) But call the parent!! (Progress can be read directly. it's a member.)
	// does not trigger OnUpdate when called directly, only when triggered by Tick.
	// Can be called from outside on purpose, skipping logic for IsActive/Loop/etc. Will let you bind this animation to other sources.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Animator, meta=(ForceAsFunction, AdvancedDisplay))
	void Update(const float Alpha);
	
	// a tick function for when you need to use this class somewhere else.
	UFUNCTION(BlueprintCallable)
	void TickManual(const float DeltaSeconds);

	// sets/clear a parent that drives the update of this animator
	UFUNCTION(BlueprintCallable)
	void BindTo(UCAnimator* const NewParent = nullptr);

	// play as is. mostly for delegates and play as set in defaults or when you only need to change one of the variables.
	UFUNCTION(BlueprintCallable, CallInEditor, meta=(AdvancedDisplay))
	FORCEINLINE void Play() { Activate(true); }
	UFUNCTION(CallInEditor, meta=(DeprecatedFunction, DevelopmentOnly)) // just to get a button for debug
	FORCEINLINE void Stop() { Deactivate(); }

	// this defines the default tick-interval. By default, it targets 60fps.
	// if the game runs at lower fps, this means it will tick every frame.
	// effectively, it's only a cap.
	// i'm still not convinced whether to use 30 or 60.
	// For now i've decided to use 60, since most of the anims are visual based (mats and transforms).
	// which have a huge impact on the quality of the game (for my personal goals).
	// still 60 is not 120 (so it will never run at 120, (maybe i should make this a config var)).
	// And then i will lower the fps (increase the interval) for those that don't need to be 60 (e.g. audio or some lights).
	inline static float IntervalDefault = 1/60.f; // cap at 60

	// to be set by game manager depending on the flags
	inline static bool Debug = false;

	// curve to use, if not set then code-curve will be used, if not set then it will be linear.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	TObjectPtr<UCurveFloat> Curve = nullptr;
	// a curve by code. only used if Curve is not set. if both not set it will be linear.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Setup|Common")
	FCodeCurve CodeCurve;

	// Duration of the animation in seconds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	float Duration = 2.0;
	// can be changed directly, but beware if doing so while playing
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	bool IsLooping = false;
	// Will restart the animation once completed.
	// If not looping, then only once. It will reset upon bouncing.
	// can be changed directly, but beware if doing so while playing.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	bool IsBouncing = false;
	// Whether currently reversed. changes on bounce.
	// Can be changed directly, but beware if doing so while playing.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	bool IsReversed = false;
	// whether to be affected or to ignore time dilation.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	bool UseDilation = true;

	// control log output per instance
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	bool UseLog = false;

	// triggers when the animation ends. but not if it wasn't playing.
	UPROPERTY(BlueprintAssignable, Transient, Category="SetUp|Signals")
	FCAnimatorRawOnEnd OnEnd;
	// when it starts, obviously
	UPROPERTY(BlueprintAssignable, Transient, Category="SetUp|Signals")
	FCAnimatorRawOnBegin OnBegin;
	// everytime the animation updates
	UPROPERTY(BlueprintAssignable, Transient, Category="SetUp|Signals")
	FCAnimatorRawOnUpdate OnUpdate;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* const ThisTickFunction) override;

	// Called when the animation ends. override me on child classes :)
	// But call the parent.
	UFUNCTION(BlueprintNativeEvent, Category=Animator, meta=(ForceAsFunction))
	void End();

	// override me on child classes :) But call the parent.
	UFUNCTION(BlueprintNativeEvent, Category=Animator, meta=(ForceAsFunction))
	void Begin();

	// called by a parent component's update
	UFUNCTION() // bound
	void ChildUpdate(const float T, const float Alpha);

	// finishes, and tentatively restart if needed.
	void Finish();
	void DoTick(float DT);
	virtual void DeInit();
	
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Transient)
	float Progress = 0.0;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UCAnimator> Parent = nullptr;
	
	float DTAcum = 0.0; // used for manual ticks
	bool HasBounced = false;
};
