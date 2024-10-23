// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Lights/LLight00.h"

#include "Light02.generated.h"

class UPointLightComponent;
class URectLightComponent;
class UCQuickMesh;

// Base class for Light02 actors. Stand lamp.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALight02: public ALLight00 {
	GENERATED_BODY()

public:
	ALight02();

protected:
	virtual void SetMobility(EComponentMobility::Type Mobility) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> Cover = nullptr;
};
