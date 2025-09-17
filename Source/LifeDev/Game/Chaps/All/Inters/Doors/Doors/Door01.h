// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Doors/Door.h"

#include "Door01.generated.h"

class UCQuickMesh;
class UStaticMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoor01: public ADoor {
	GENERATED_BODY()

public:
	ADoor01();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCQuickMesh* Window = nullptr;
};
