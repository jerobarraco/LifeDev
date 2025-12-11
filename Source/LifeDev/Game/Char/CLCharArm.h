// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"

#include "CLCharArm.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UCLCharArm : public USpringArmComponent {
	GENERATED_BODY()

public:
	UCLCharArm();
};
