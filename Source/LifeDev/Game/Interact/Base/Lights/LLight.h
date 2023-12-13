// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "JSig/CSignificance.h"
#include "LifeDev/Game/Interact/LInteract.h"

#include "LLight.generated.h"

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
	
	UFUNCTION()
	void TurnOn();
	UFUNCTION()
	void SetFB(float Value);

	// flickers when fb is >= this value. <0 means disabled.
	UPROPERTY(BlueprintReadOnly, Category=SetUp)
	float FlickrOnFB = -1;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCRandomizer* Rnd = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCSignificance* Sig = nullptr;
};
