// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Lights/LLight00.h"

#include "Light01.generated.h"

class UPointLightComponent;
class URectLightComponent;
class UCQuickMesh;

// Base class for Light01 actors. Glass Turtle light
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALight01: public ALLight00 {
	GENERATED_BODY()

public:
	ALight01();

protected:
	virtual void SetMobility(EComponentMobility::Type Mobility) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCQuickMesh* Glass = nullptr;
};
