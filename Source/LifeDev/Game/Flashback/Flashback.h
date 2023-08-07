// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "Flashback.generated.h"

class UCAnimator;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFBOnChange, float, Value);

// Base subsystem for flashbacks
UCLASS(Blueprintable, Category="LifeDev")
class LIFEDEV_API UFlashback : public UTickableWorldSubsystem
{
private:
	GENERATED_BODY()

public:
	UFlashback();

	UFUNCTION(BlueprintCallable)
	float GetVal() const { return Val; }

	UFUNCTION(BlueprintCallable)
	void IncVal(float By, float Speed = -1.f);

	// The time is the time to go from 0 to 1. it will be proportional to the difference so the speed is always the same.
	// if it's <0 it will use the default time. 0 will be instant. >0 will use that.
	UFUNCTION(BlueprintCallable)
	void SetVal(float New, float Speed = -1.f);
	
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FFBOnChange OnChange;

protected:
	void SetValInternal(float New);
	UFUNCTION() // for binding
	void AnimUpdate(float Progress, float Alpha);
	
	float Val = 0;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCAnimator* Animator;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float AnimSpeed = 20.f;

	float AnimFrom = 0;
	float AnimTo = 0;
};
