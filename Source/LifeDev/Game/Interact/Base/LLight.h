// Copyright Jerónimo Barraco-Mármol

#pragma once

#include "LifeDev/Game/Interact/LInteract.h"

#include "LLight.generated.h"

class UCRandomizer;
// Base class for LLight actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALLight: public ALInteract {
	GENERATED_BODY()

public:
	ALLight();

	// don´t call before begin play.
	UFUNCTION(BlueprintCallable)
	void SetFBFlicker(float NewFBFlicker);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	void SetFB(float Value);
	// flickers when fb is >= this value. <0 means disabled.
	UPROPERTY(BlueprintReadOnly, Category=SetUp)
	float FlickrOnFB = -1;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCRandomizer* Rnd = nullptr;
};
