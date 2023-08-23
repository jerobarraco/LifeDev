// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once

#include "CNoiser.generated.h"

class UCurveFloat;
class USceneComponent;
class USoundAttenuation;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCNoiserRawOnPlay);

// Plays a sound at random intervals, at a random distance, on a random point in a cone facing on the back of the owner actor.
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class SOUNDS_API UCNoiser: public UActorComponent {
	GENERATED_BODY()
public:

	UCNoiser();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Start();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Stop();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	inline bool GetIsPlaying() const { return IsPlaying; }

	// will start or stop the noises accordingly
	UFUNCTION(BlueprintCallable)
	void SetIsPlaying(bool NewIsRotating);
	
	// Half Radius of the one facing back of the actor. On the horizontal axis. Degrees
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	float HalfAngleWidth = 120.0;
	
	// Half Radius of the one facing back of the actor. On the vertical axis. Degrees
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	double HalfAngleHeight = 30;

	// Minimum distance from the actor
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	float DistMin = 10;
	// Maximum distance from the actor
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
	// whether to show the debug lines 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Debug")
	bool Debug = false;

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, CallInEditor)
	void PlayNow();
	virtual void PlayNow_Implementation();

	void TimerStop();
	void TimerStart();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	bool IsPlaying = false;

	FTimerHandle Handle;
};
