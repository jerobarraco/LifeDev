// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "Flashback.generated.h"

class UCAnimator;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFBOnChange, float, Value);

// Base subsystem for flashbacks
UCLASS(Blueprintable, Category="LifeDev")
class LIFEDEV_API UFlashback: public UTickableWorldSubsystem {
	GENERATED_BODY()

public:
	UFlashback();

	static UFlashback* Get(UWorld* W);

	UFUNCTION(BlueprintCallable)
	static void SetValS(UWorld* W, float New, float Duration = -1.f);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetVal() const { return Val; }

	// modifies the target value by an offset "by".
	// this will work ok even if it's currently animating the value
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void ModVal(float By, float Duration = -1.f) {
		if (FMath::IsNearlyZero(By)) return;
		SetVal(Val+By, Duration);
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
	// You can set it >1 but it might break stuff i recommend keep in range [0,1]
	UFUNCTION(BlueprintCallable)
	void SetMax(float NewMax, float Duration=1.f);
	// Sets the min, and clamp the target value if out of range.
	// You can set it <0 but it might break stuff i recommend keep in range [0,1]
	UFUNCTION(BlueprintCallable)
	void SetMin(float NewMin, float Duration=1.f);

	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FFBOnChange OnChange;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float AnimSpeed = 20.f;
	
	inline static bool Debug = false;

protected:
	// doesn't check the range
	void SetValInternal(float New);
	UFUNCTION() // for binding
	void AnimUpdate(float Progress, float Alpha);
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCAnimator* Animator = nullptr;

	// it's super important that the value starts from 0 upon initialization
	float Val = 0;
	float AnimFrom = 0;
	float AnimTo = 0;
	float Min = 0;
	float Max = 1;
};
