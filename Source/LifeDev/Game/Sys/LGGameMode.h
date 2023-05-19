// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "LGGameMode.generated.h"

class UInputMappingContext;
class ADialogManager;

// Game mode class 
UCLASS(Blueprintable)
class LIFEDEV_API ALGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALGGameMode();

	UFUNCTION(BlueprintCallable)
	void Init();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Transient)
	ADialogManager* DiagManager = nullptr;
};



