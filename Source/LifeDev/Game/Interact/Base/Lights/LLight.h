// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "LifeDev/Game/Interact/LInteract.h"

#include "LLight.generated.h"

class UCLSignificance;
class UCRandomizer;
// Remember to set cast shadow on the correct meshes

// Base class for light actors. doesn't include the light itself. see LLight00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALLight: public ALInteract {
	GENERATED_BODY()

public:
	ALLight();

	// don't call before begin play.
	UFUNCTION(BlueprintCallable, meta=(UnsafeDuringActorConstruction))
	void SetFBFlicker(float NewFBFlicker);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AnimUpdate(float P, float A);
	virtual void AnimUpdate_Implementation(float P, float A) {}

	virtual void SetState_Implementation(int32 NewState) override;
	
	UFUNCTION() // bind
	void TurnOn();
	UFUNCTION() // bind
	void SetFB(float Value);

	// flickers when fb is >= this value. <0 means disabled.
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	float FlickrOnFB = -1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCRandomizer* Rnd = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCLSignificance* Sig = nullptr;
};
