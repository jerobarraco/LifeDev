// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "LifeDev/Game/Chaps/All/NPCs/LNPC01.h"
#include "LifeDev/Game/Flashback/Flashback.h"

#include "LNPC01I00.generated.h"

class UCAnimatorCam;
// TODO move stuff from base to here
// the instance for the npc01 in chapter 01 at boss step 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ALNPC01I00 : public ALNPC01 {
	GENERATED_BODY()

public:
	ALNPC01I00();

protected:
	virtual void BeginPlay() override;
	virtual void TriggerLocked_Implementation() override;
	virtual EItemUseResult TryUseItem_Implementation(const FName& Name) override;
	void StandUp();

	UFUNCTION()
	void DiagStandDone();
	UFUNCTION()
	void DiagSitDone();
	UFUNCTION()
	void FadeDone();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UCAnimatorCam* AnimCam = nullptr;
	// UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	// UCAnimatorFade* AnimFade = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	UFlashback* Flashback = nullptr;
};
