// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once

#include "CNoiser.generated.h"

class UCurveFloat;
class USceneComponent;
class USoundAttenuation;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCNoiserRawOnEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCNoiserRawOnPlay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCNoiserRawOnUpdate, float, Progress, float, Alpha);

// An interactive actor that can have an animation
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCNoiser: public UActorComponent {
	GENERATED_BODY()
public:

	UCNoiser();
	void TimerStop();
	void TimerStart();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Start();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Stop();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline bool GetIsPlaying() const { return IsPlaying; }

	// Half Radius of the one facing back of the actor
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	float HalfRadius = 120.0;

	// Maximum distance to the actor
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	float DistMax = 120.0;

	// Min time between sounds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	float TimeMin = 2.0;
	// Max sound between sounds
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	float TimeMax = 5.0;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	USoundBase* SFX = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	USoundAttenuation* Attenuation = nullptr;

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp|Signals")
	FCNoiserRawOnPlay OnPlay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Signals")
	bool Debug = true;

protected:
	UFUNCTION(BlueprintCallable)
	void SetIsPlaying(bool NewIsRotating);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, CallInEditor)
	void PlayNow();
	virtual void PlayNow_Implementation();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsPlaying = false;

	FTimerHandle Handle;
};
