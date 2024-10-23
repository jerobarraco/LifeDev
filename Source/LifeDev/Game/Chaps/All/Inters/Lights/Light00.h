// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Lights/LLight01.h"

#include "Light00.generated.h"

class URectLightComponent;
class UCQuickMesh;

// Base Fluorescent, inherit from base rect light.
// LLight are general lights. Light are not so general.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALight00: public ALLight01 {
	GENERATED_BODY()

public:
	ALight00();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetMobility(EComponentMobility::Type Mobility) override;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCQuickMesh> Tube = nullptr;
};
