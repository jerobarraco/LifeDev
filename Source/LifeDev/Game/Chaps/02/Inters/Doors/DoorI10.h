// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Doors/Doors/Door02.h"

#include "DoorI10.generated.h"

// Door from hall00 to piano
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoorI10: public ADoor02 {
	GENERATED_BODY()

public:
	ADoorI10();
	virtual void DoTrigger_Implementation() override;

protected:
	UFUNCTION()
	void DoDialog();
	UFUNCTION()
	void Shoot();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_Gun = nullptr;
};
