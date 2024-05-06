// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "PianoLid.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API APianoLid: public ALInteract {
	GENERATED_BODY()

public:
	APianoLid();

protected:
	virtual void PostInitializeComponents() override;
};
