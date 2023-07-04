// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "IntroGameMode.generated.h"

class AIntroMan;

// Game mode class for the intro 
UCLASS(Blueprintable)
class LIFEDEV_API AIntroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIntroGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Transient)
	AIntroMan* Manager = nullptr;
};
