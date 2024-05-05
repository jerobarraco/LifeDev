// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Chaps/All/Inters/Papers/Calendar.h"

#include "CharNRG.generated.h"

class UNiagaraComponent;

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ACharNRG: public ALInteract {
	GENERATED_BODY()

public:
	ACharNRG();

protected:
	virtual void BeginPlay() override;
	virtual void SetEnabled_Implementation(bool Enabled) override;
	virtual void AnimEnd_Implementation() override;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UNiagaraComponent* Parts = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="SetUp|Nrg")
	float SpawnRateMax = 30;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="SetUp|Nrg")
	float SpawnRateMin = 2;
};
