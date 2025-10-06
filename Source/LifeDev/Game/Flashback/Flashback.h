// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "Flashback.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFBOnChange, const float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFBOnTo, const float, To);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFBOnEnd, const float, Value);

class UCAnimator;

// Base subsystem for flashbacks
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig, Category="LifeDev")
class LIFEDEV_API UFlashback: public UTickableWorldSubsystem {
	GENERATED_BODY()

public:
	UFlashback();

	static UFlashback* Instance(const UObject* const O);

	UFUNCTION()
	void Init() {}; // for the game mode
	
	// returns the instant value (if it's animating this is the value right now).
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetVal() const { return Val; }

	// returns the end value (if it's animating this is the value where it wants to go).
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetValTo() const { return ValTo; }
	
	// modifies the target value by an offset "by".
	// this will work ok even if it's currently animating the value
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ModVal(const float By, const float Duration = -1.f) {
		if (FMath::IsNearlyZero(By)) return;
		// use animTo instead of val, to ensure the By accumulates with the target value
		// important when skipping dialogs fast
		SetVal(ValTo+By, Duration);
	}

	// sets the target value to the "new" value
	// The "Duration" is the time to go from 0 to 1.
	// Then it will be proportional to the difference, so the speed is always the same. (Time = Duration*Diff)
	//	if it's 0 will be instant.
	//	<0 it will use the default time (set on AnimSpeed).
	//	>0 will use whatever value it is.
	UFUNCTION(BlueprintCallable)
	void SetVal(float New, float Duration = -1.f);

	// Sets the max, and clamp the target value if out of range.
	// You can set it >1, but it might break stuff i recommend keep in range [0,1]
	// duration works like in SetVal
	UFUNCTION(BlueprintCallable)
	void SetMax(const float NewMax, const float Duration=-1.f);
	// Sets the min, and clamp the target value if out of range.
	// You can set it <0, but it might break stuff i recommend keep in range [0,1]
	// duration works like in SetVal
	UFUNCTION(BlueprintCallable)
	void SetMin(const float NewMin, const float Duration=-1.f);

	static constexpr float DefAnimTime = 20.f;
	// default flashback anim speed
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=SetUp, Config)
	float AnimTime = DefAnimTime;

	// triggered with each value update (updates per tick).
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FFBOnChange OnChange;
	
	// Triggered on a new target value. Only once.
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FFBOnTo OnTo;

	// Triggered on done.
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Transient)
	FFBOnEnd OnEnd;

	inline static bool Debug = false;

protected:
#pragma region base
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
#pragma endregion

	// doesn't check the range.
	void SetValToNow(const float New);
	// doesn't check the range
	void SetValNow(const float New);
	UFUNCTION() // for binding
	void AnimUpdate(const float Progress, const float Alpha);
	UFUNCTION() // bind
	void AnimEnd();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCAnimator> Animator = nullptr;

	// it's super important that the value starts from 0 upon initialization
	float Val = 0;
	float ValFrom = 0;
	float ValTo = 0;
	float Min = 0;
	float Max = 1;
};
