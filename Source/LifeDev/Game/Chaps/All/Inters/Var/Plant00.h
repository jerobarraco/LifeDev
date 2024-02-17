// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Plant00.generated.h"


// Base Plant00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APlant00: public ALInteract {
	GENERATED_BODY()

public:
	APlant00();

protected:
	virtual void SetMobility(EComponentMobility::Type Mobility) override;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCQuickMesh* Soil = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCQuickMesh* Plant = nullptr;
};
