// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Kitchen/Stove.h"

#include "StoveI00.generated.h"

// instance for StoveI00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AStoveI00: public AStove {
	GENERATED_BODY()

public:
	AStoveI00();

protected:
	virtual void DoTrigger_Implementation() override;

	// matches snd
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|SFX")
	TObjectPtr<USoundBase> SFXTriggerB = nullptr;
};
