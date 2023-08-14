// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "LifeDev/Game/NPCs/LNPCH.h"

#include "LNPC01.generated.h"

// TODO move stuff from here to the child npc01I00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALNPC01 : public ALNPCH {
	GENERATED_BODY()

public:
	ALNPC01();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetPoseStand();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetPoseSit();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SetVisible(bool Vis);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* Card = nullptr;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FNPCHPose PoseSit;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FNPCHPose PoseStand;
};
