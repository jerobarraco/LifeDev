// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LLight.h"

#include "LLight00.generated.h"

class UPointLightComponent;
class URectLightComponent;

// Base class for point light actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALLight00: public ALLight {
	GENERATED_BODY()

public:
	ALLight00();

protected:
	virtual void SetMobility(const EComponentMobility::Type Mobility) override;
	virtual void AnimUpdate_Implementation(const float P, const float A) override;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UPointLightComponent> Light = nullptr;
};
