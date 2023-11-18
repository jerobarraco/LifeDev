// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Var/Radio00.h"

#include "RadioI00.generated.h"

class UCSounder;

// Radio00 Instance 00. Room 03
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ARadioI00: public ARadio00 {
	GENERATED_BODY()

public:
	ARadioI00();

protected:
	virtual void Trigger_Implementation() override;

	UFUNCTION()
	void DialogDone();
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCSounder* SFX_Ghost = nullptr;

	FTimerHandle DiagDoneHandle;
};
