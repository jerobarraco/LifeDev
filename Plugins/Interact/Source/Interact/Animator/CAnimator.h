// Copyright (C) 2023 - Jerónimo Barraco-Mármol

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
	
	// a tick function for when you need to use this class somewhere else.
	UFUNCTION(BlueprintCallable)
	void TickManual(float DeltaSeconds);

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
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Signals")
	FCAnimatorRawOnEnd OnEnd;
	// when it starts, obviously
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Signals")
	FCAnimatorRawOnBegin OnBegin;
	// everytime the animation updates
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Signals")
	FCAnimatorRawOnUpdate OnUpdate;

	// This is just experimental and can be removed at any point
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Setup")
	TArray<UAnimTrackBase*> Tracks;

	// experimental too. For some reason this only works after begin play and not on constructor.
	UFUNCTION(BlueprintCallable)
	void AddTrackMatF(UMaterialInstanceDynamic* M, const FName Name, float FStart, float FEnd);

	UFUNCTION(BlueprintCallable)
	void BindTo(UCAnimator* Parent);

	

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

	UFUNCTION()
	void ChildUpdate(float T, float Alpha);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void DeInit();
	void DoTick(float DeltaSeconds);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UCAnimator* Parent = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsAnimating = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	float Progress = 0.0;
	
	float DTAcum = 0.0; // used for manual ticks 
};
