// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Var/Tape00.h"

#include "TapeI01.generated.h"

// Tape00 Instance 01. this finishes the c1s1
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ATapeI01: public ATape00 {
	GENERATED_BODY()

public:
	ATapeI01();

protected:
	virtual void Trigger_Implementation() override;
};
