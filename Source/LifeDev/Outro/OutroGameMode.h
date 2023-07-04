// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "OutroGameMode.generated.h"

class AOutroMan;

// Game mode class for the outro 
UCLASS(Blueprintable)
class LIFEDEV_API AOutroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOutroGameMode();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Transient)
	AOutroMan* Manager = nullptr;
};
