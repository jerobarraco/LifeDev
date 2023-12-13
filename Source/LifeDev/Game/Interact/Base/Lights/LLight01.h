// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Lights/LLight.h"

#include "LLight01.generated.h"

class URectLightComponent;
class UCQuickMesh;

// Base class for rect lights. TODO finish
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALLight01: public ALLight {
	GENERATED_BODY()

public:
	ALLight01();

protected:
	UFUNCTION() // bind
	void UpdateAnim(float Progress, float Alpha);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetMobility(EComponentMobility::Type Mobility) override;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	URectLightComponent* RectLight = nullptr;
};
