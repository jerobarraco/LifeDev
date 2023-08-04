// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LifeDev/Game/Interact/Base/Window.h"

#include "Window00.generated.h"

// Window 00 Window type 0 side 0
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AWindow00: public AWindow {
	GENERATED_BODY()
public:

	AWindow00();

protected:
	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* GlassA;
	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* GlassB;
	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* GlassC;
};
