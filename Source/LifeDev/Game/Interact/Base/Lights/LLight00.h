// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LLight.h"

#include "LLight00.generated.h"

class UPointLightComponent;
class URectLightComponent;

// Base class for point light actors with a point light
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALLight00: public ALLight {
	GENERATED_BODY()

public:
	ALLight00();

protected:
	virtual void BeginPlay() override;
	virtual void SetMobility(const EComponentMobility::Type Mobility) override;
	virtual void AnimUpdate_Implementation(const float Progress, const float Alpha) override;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UPointLightComponent> Light = nullptr;
};
