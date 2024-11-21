// Copyright Jerónimo Barraco-Mármol
#pragma once

#include "LifeDev/Game/Interact/LInteract.h"

#include "Tape00.generated.h"

class UCQuickMesh;

// Base Tape00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ATape00: public ALInteract {
	GENERATED_BODY()

public:
	ATape00();

protected:
	virtual void SetMobility(EComponentMobility::Type Mobility) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> Case = nullptr;
};
