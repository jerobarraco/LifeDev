// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Game/Chaps/All/NPCs/LNPC04.h"

#include "LNPCI00.generated.h"

class UCAnimatorCam;

// the instance for the npc01 in chapter 01 at boss step 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALNPCI00 : public ALNPC04 {
	GENERATED_BODY()

public:
	ALNPCI00();
};
