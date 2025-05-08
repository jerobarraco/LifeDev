// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Var/Batts.h"

#include "BattsI01.generated.h"

// Batts Instance 01. chap02 starts hidden (due to how they are located)
// the steps enables it
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABattsI01: public ABatts {
	GENERATED_BODY()

public:
	ABattsI01();
};
