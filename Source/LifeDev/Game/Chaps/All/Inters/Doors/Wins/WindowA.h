// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LifeDev/Game/Interact/Base/Window.h"

#include "WindowA.generated.h"

// Window 00 Window type 0 side 0
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AWindowA: public AWindow {
	GENERATED_BODY()
public:

	AWindowA();

protected:
	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* GlassA;
	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* GlassB;
	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* GlassC;
};
