// Copyright 2023 Jerónimo Barraco-Mármol

#pragma once

#include "LifeDev/Game/Interact/LInteract.h"

#include "Radio00.generated.h"

// Base Radio00. not active by default. starts closed.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ARadio00: public ALInteract {
	GENERATED_BODY()

public:
	ARadio00();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> Door = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<USceneComponent> DoorRoot = nullptr;
};
