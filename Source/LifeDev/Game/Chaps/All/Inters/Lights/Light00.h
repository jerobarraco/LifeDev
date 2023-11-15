// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/LLight.h"

#include "Light00.generated.h"

class URectLightComponent;
class UCQuickMesh;

// Base class for Light00 actors
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
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCQuickMesh* Tube = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	URectLightComponent* RectLight = nullptr;
};
