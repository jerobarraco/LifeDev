// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Lights/LLight.h"

#include "LLight02.generated.h"

class USpotLightComponent;

// Base class for spot lights.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALLight02: public ALLight {
	GENERATED_BODY()

public:
	ALLight02();

protected:
	virtual void SetMobility(EComponentMobility::Type Mobility) override;
	virtual void AnimUpdate_Implementation(float P, float A) override;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USpotLightComponent> Light = nullptr;
};
