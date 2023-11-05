// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Radio00.generated.h"

// Base Radio00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ARadio00: public ALInteract {
	GENERATED_BODY()

public:
	ARadio00();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCQuickMesh* Base = NULL;
};
