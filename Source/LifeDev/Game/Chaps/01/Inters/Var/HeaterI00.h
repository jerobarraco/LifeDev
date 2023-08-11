// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Doors/Var/Heater00.h"

#include "HeaterI00.generated.h"

// Heater00 Instance 00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AHeaterI00: public AHeater00 {
public:
	GENERATED_BODY()

	AHeaterI00();

	virtual void Trigger_Implementation() override;
	virtual void TriggerLocked_Implementation() override;
};
