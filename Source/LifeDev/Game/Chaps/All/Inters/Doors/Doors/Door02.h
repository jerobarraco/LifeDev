// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Door.h"

#include "Door02.generated.h"

class UCQuickMesh;
class UStaticMeshComponent;

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoor02: public ADoorish {
	GENERATED_BODY()

public:
	ADoor02();

protected:
	virtual void SetMobility(EComponentMobility::Type Mobility) override;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCQuickMesh* WinA = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	UCQuickMesh* WinB = nullptr;
};
