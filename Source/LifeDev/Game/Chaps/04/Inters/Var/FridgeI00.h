// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Kitchen/Fridge.h"

#include "FridgeI00.generated.h"

// Fridge instance on chap04
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AFridgeI00: public AFridge {
	GENERATED_BODY()

public:
	AFridgeI00();
	virtual void DoTrigger_Implementation() override;
};
