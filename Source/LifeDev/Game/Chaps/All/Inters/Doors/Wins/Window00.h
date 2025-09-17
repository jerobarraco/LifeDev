// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"
#include "LifeDev/Game/Interact/Base/Doors/Window.h"

#include "Window00.generated.h"

// Window 00 Window type 0 side 0
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AWindow00: public AWindow {
	GENERATED_BODY()

public:
	AWindow00();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> GlassA = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> GlassB = nullptr;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCQuickMesh> GlassC = nullptr;
};
