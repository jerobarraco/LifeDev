// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "LifeDev/Game/Interact/Base/Drawer.h"

#include "Window00.generated.h"

// Window 00 Window type 0 side 0
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AWindow00: public ADrawer {
public:
	GENERATED_BODY()

	AWindow00();

protected:
	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* GlassA;
	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* GlassB;
	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* GlassC;
};
