// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Game/Chaps/All/NPCs/LNPC01.h"

#include "LNPCI01.generated.h"

class UCAnimatorCam;

// the instance for the npc01 in chapter 01 at boss step 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALNPCI01 : public ALNPC01 {
	GENERATED_BODY()

public:
	ALNPCI01();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void DoTrigger_Implementation() override;
	void StandUp();
};
