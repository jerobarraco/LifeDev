// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "Inventory/InventoryManager.h"

#include "LGGameMode.generated.h"

class UInputMappingContext;
class ADialogManager;
class AInventoryManager;

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
	void DeInit();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	ADialogManager* DiagManager = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	AInventoryManager* InvManager = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	int32 Chapter = 1; // Maybe this could be on the game instance. ?
};
