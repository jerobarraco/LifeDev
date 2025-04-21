// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/LInteract.h"

#include "Basin00.generated.h"

class UCLSounder;
class UNiagaraComponent;
class UCSignificance;

// basin00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABasin00: public ALInteract {
	GENERATED_BODY()

public:
	ABasin00();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCSignificance> Sig = nullptr;
};
