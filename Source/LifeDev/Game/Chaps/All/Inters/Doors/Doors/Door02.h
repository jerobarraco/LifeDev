// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Doors/Doorish.h"

#include "Door02.generated.h"

class UCQuickMesh;
class UStaticMeshComponent;

// base Double door - Left side. Inherits doorish since Door has a frame
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ADoor02: public ADoorish {
	GENERATED_BODY()

public:
	ADoor02();

protected:
	virtual void SetMobility(const EComponentMobility::Type Mobility) override;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCQuickMesh> WinA = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UCQuickMesh> WinB = nullptr;
};
