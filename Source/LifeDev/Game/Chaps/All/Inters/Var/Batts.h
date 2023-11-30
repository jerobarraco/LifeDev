// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Batts.generated.h"

// Base class for Batts. extend for instances.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABatts: public ALInteract {
	GENERATED_BODY()

public:
	ABatts();
	virtual void SetMobility(EComponentMobility::Type Mobility) override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCQuickMesh* MeshB = nullptr;
};
