// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Game/Chaps/All/NPCs/LNPC02.h"

#include "LNPCI01.generated.h"

class UCAnimatorCam;
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALNPCI01 : public ALNPC02 {
	GENERATED_BODY()

public:
	ALNPCI01();
	void BeginPlay();

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UCAnimatorCam* AnimCam = nullptr;
};
