// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "LGMGame.generated.h"

class UInputMappingContext;

// Game mode class 
UCLASS(Blueprintable)
class LIFEDEV_API ALGMGame : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALGMGame();

	virtual void BeginPlay() override;
};



