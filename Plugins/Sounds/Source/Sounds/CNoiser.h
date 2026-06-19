// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "Components/ActorComponent.h"

#include "CNoiser.generated.h"

class USoundClass;
class UCurveFloat;
class USceneComponent;
class USoundAttenuation;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCNoiserRawOnPlay);

// Plays a sound at random intervals, at a random distance, on a random point in a cone facing on the back of the owner actor.
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Sounds), meta=(BlueprintSpawnableComponent))
class SOUNDS_API UCNoiser: public UActorComponent {
	GENERATED_BODY()

public:
	UCNoiser();

	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;
	
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
	float TimeMax = 10.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	TObjectPtr<USoundBase> SFX = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	TObjectPtr<USoundAttenuation> Attenuation = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	TObjectPtr<USoundClass> SoundClass = nullptr;

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

	FTimerHandle TimerPlay;
};
