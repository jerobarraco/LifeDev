// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Game/Chaps/All/NPCs/LNPC01.h"

#include "LNPC01I00.generated.h"

class UCAnimatorCam;

// the instance for the npc01 in chapter 01 at boss step 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALNPC01I00 : public ALNPC01 {
	GENERATED_BODY()

public:
	ALNPC01I00();

protected:
	virtual void BeginPlay() override;
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name) override;
	void StandUp();

	UFUNCTION()
	void DiagStandDone();
	UFUNCTION()
	void DiagSitDone();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCAnimatorCam> AnimCam = nullptr;
};
