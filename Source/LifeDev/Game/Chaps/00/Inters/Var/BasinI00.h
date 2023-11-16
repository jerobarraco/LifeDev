// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "BasinI00.generated.h"

class UNiagaraComponent;
class UCSounder;

// Ch00 basin00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABasinI00: public ALInteract {
	GENERATED_BODY()

public:
	ABasinI00();

protected:
	virtual void Trigger_Implementation() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UNiagaraComponent* Water = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UCSounder* SND_Water = nullptr;
};
