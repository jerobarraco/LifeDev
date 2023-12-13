// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Lights/LLight.h"

#include "Light00.generated.h"

class URectLightComponent;
class UCQuickMesh;

// TODO make a base for rect lights that's ALLight01, see ALLight00
// Base Fluorescent
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALight00: public ALLight {
	GENERATED_BODY()

public:
	ALight00();

protected:
	UFUNCTION() // bind
	void UpdateAnim(float Progress, float Alpha);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetMobility(EComponentMobility::Type Mobility) override;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCQuickMesh* Tube = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	URectLightComponent* RectLight = nullptr;
};
