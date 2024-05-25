// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "Diags/DiagTypes.h"
#include "LifeDev/Game/Chaps/All/Inters/Var/Radio00.h"

#include "RadioI00.generated.h"

class UCLSounder;
class UCSounder;

// Radio00 Instance 00. Room 03
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ARadioI00: public ARadio00 {
	GENERATED_BODY()

public:
	ARadioI00();

protected:
	virtual void DoTrigger_Implementation() override;

	UFUNCTION()
	void DialogDone();
	UFUNCTION()
	void DialogShown(const FDialog& Diag);
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCLSounder* SFX_Ghost = nullptr;

	FTimerHandle DiagDoneHandle;
};
