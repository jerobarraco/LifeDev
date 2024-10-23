// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Lights/LLight.h"

#include "LLight01.generated.h"

class URectLightComponent;
class UCQuickMesh;

// Base class for rect lights.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALLight01: public ALLight {
	GENERATED_BODY()

public:
	ALLight01();

protected:
	virtual void SetMobility(EComponentMobility::Type Mobility) override;
	virtual void AnimUpdate_Implementation(float P, float A) override;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<URectLightComponent> RectLight = nullptr;
};
