// Copyright Jerónimo Barraco-Mármol

#pragma once
#include "CoreMinimal.h"

#include "LifeDev/Game/Interact/Base/Doorish.h"

#include "Window.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API AWindow: public ADoorish {
	GENERATED_BODY()

public:
	AWindow();
};
