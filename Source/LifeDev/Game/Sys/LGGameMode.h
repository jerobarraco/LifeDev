// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LifeDev/Game/Char/LCharacter.h"

#include "LGGameMode.generated.h"

class UInputMappingContext;
class ADialogManager;
class AInventoryManager;
class AStoryManager;

// Game mode class 
UCLASS(Blueprintable)
class LIFEDEV_API ALGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALGGameMode();

	UFUNCTION(BlueprintCallable)
	static ALGGameMode* Get();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	virtual void Init_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeInit();
	virtual void DeInit_Implementation();

	UFUNCTION(BlueprintCallable)
	void SetCharInputEnabled(bool Enabled);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, Transient)
	ADialogManager* DiagManager = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	AInventoryManager* InvManager = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	AStoryManager* StoryManager = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	ALCharacter* Char = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	int32 Chapter = 1; // Maybe this could be on the game instance. ?

protected:
	void StartStory() const;
};
