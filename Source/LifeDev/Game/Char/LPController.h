// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "LPController.generated.h"

// LifeDev PlayerController
UCLASS(Blueprintable)
class LIFEDEV_API ALPController : public APlayerController {
	GENERATED_BODY()
	virtual void BeginPlay() override;
};
