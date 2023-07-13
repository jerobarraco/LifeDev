// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Door.h"

#include "Door01.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoor01: public ADoor {
public:
	GENERATED_BODY()

	ADoor01();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UStaticMeshComponent* Window;
};
