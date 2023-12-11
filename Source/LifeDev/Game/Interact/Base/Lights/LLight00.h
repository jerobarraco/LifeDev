// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LLight.h"

#include "LLight00.generated.h"

class UPointLightComponent;
class URectLightComponent;
class UCQuickMesh;

// Base class for point light actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALLight00: public ALLight {
	GENERATED_BODY()

public:
	ALLight00();

protected:
	UFUNCTION() // bind
	void AnimLight(float Progress, float Alpha);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetMobility(EComponentMobility::Type Mobility) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float IntensityMax = 2;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UPointLightComponent* Light = nullptr;
};
