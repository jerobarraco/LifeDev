// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "LifeDev/Game/NPCs/LNPCH.h"

#include "LNPC02.generated.h"

// TODO create ALNPCI02
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALNPC02 : public ALNPCH {
	GENERATED_BODY()

public:
	ALNPC02();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetPoseStand();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetPoseSit();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetVisible(bool Vis);
	
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FNPCHPose PoseSit;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FNPCHPose PoseStand;
};
