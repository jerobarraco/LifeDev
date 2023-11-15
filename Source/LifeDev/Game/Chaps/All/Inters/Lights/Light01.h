// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LLight.h"

#include "Light01.generated.h"

class UPointLightComponent;
class URectLightComponent;
class UCQuickMesh;

// Base class for Light01 actors
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALight01: public ALLight {
	GENERATED_BODY()

public:
	ALight01();

protected:
	UFUNCTION() // bind
	void UpdateAnim(float Progress, float Alpha);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetMobility(EComponentMobility::Type Mobility) override;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float IntensityMax = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCQuickMesh* Glass = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UPointLightComponent* Light = nullptr;
};
