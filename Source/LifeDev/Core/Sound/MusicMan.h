// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once

#include "MusicMan.generated.h"

class UCurveFloat;
class USceneComponent;
class USoundAttenuation;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCMusicManRawOnPlay);

// Plays a sound at random intervals, at a random distance, on a random point in a cone facing on the back of the owner actor.
UCLASS(Blueprintable, BlueprintType,Placeable, ClassGroup=(LifeDev), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API AMusicMan: public AActor {
	GENERATED_BODY()

public:
	AMusicMan();
	inline static bool Enabled = true;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetIntensity(float V);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Common")
	UAudioComponent* Player = nullptr;
};
