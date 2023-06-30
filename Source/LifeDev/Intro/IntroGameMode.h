// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "IntroGameMode.generated.h"


// Game mode class for the intro 
UCLASS(Blueprintable)
class LIFEDEV_API AIntroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIntroGameMode();
};
