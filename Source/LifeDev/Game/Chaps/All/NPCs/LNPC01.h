// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "LifeDev/Game/NPCs/LNPCH.h"

#include "LNPC01.generated.h"

// TODO have a base class for ALNPC1 and a child for ALNPC01I1 with the dialogs and sounds
UCLASS(Blueprintable, BlueprintType)
class ALNPC01 : public ALNPCH {
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
	void TriggerLocked_Implementation();
	
	UFUNCTION()
	void DiagStandDone();
	UFUNCTION()
	void DiagSitDone();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FNPCHPose PoseSit;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FNPCHPose PoseStand;
};
