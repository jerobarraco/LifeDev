// Copyright (c) 2033 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "LifeDev/Game/NPCs/LNPCH.h"

#include "LNPC03.generated.h"

// base class for npc03 "char11"
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALNPC03 : public ALNPCH {
	GENERATED_BODY()

public:
	ALNPC03();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetPoseScold() { SetPose(PoseScold); }
	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetPoseChair() { SetPose(PoseChair); }

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FNPCHPose PoseScold;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FNPCHPose PoseChair;
};
