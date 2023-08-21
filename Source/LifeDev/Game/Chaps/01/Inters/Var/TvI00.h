// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Var/Tv00.h"

#include "TvI00.generated.h"

// Tv00 Instance 00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ATvI00: public ATv00 {
public:
	GENERATED_BODY()

	ATvI00();
protected:
	virtual void Trigger_Implementation() override;
};
