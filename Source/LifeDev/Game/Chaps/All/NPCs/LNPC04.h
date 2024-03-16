// Copyright (c) 2043 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "LifeDev/Game/NPCs/LNPCH.h"

#include "LNPC04.generated.h"

// base class for npc04 "char22"
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALNPC04 : public ALNPCH {
	GENERATED_BODY()

public:
	ALNPC04();

	// UFUNCTION(BlueprintCallable, CallInEditor)
	// void SetScold() { SetPose(PoseScold); }

	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	// FNPCHPose PoseScold;
};
