// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "JSig/CSignificance.h"
#include "LifeDev/Game/Interact/LInteract.h"

#include "BasinI00.generated.h"

class UNiagaraComponent;
class UCSounder;
class UCSignificance;

// Ch00 basin00
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ABasinI00: public ALInteract {
	GENERATED_BODY()

public:
	ABasinI00();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Trigger_Implementation() override;
	UFUNCTION()
	void SigChanged(ESignificance Significance);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UNiagaraComponent* Water = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCSounder* SND_Water = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UCSignificance* Sig = nullptr;
};
