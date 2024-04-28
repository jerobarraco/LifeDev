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
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UNiagaraComponent* Parts = nullptr;


};
