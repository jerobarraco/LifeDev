// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LifeDev/Core/Settings/FLChapter.h"
#include "Dialogs/DiagTypes.h"
#include "LGGameMode.generated.h"

class UInputMappingContext;
class ADialogManager;
class AInventoryManager;
class AStoryManager;
class ULSysSettings;
class ALCharacter;
class UStory;
class UDialogs;

// to get the collision channel read https://gamedev.stackexchange.com/a/178812/44866
#define InteractTraceChannel ECC_GameTraceChannel1

// Game mode class 
UCLASS(Blueprintable)
class LIFEDEV_API ALGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALGGameMode();
	bool LoadChapter();

	UFUNCTION(BlueprintCallable)
	static ALGGameMode* Get();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	virtual void Init_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeInit();
	virtual void DeInit_Implementation();

	// enables or disables char input. and stays like that even though of dialogs.
	UFUNCTION(BlueprintCallable)
	void SetCharInputEnabled(bool Enabled);

	// disables input temporarily, enables only if char is enabled. used for dialogs.
	UFUNCTION(BlueprintCallable)
	void SetTempInputEnabled(bool Enabled);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	ADialogManager* DiagManager = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	AInventoryManager* InvManager = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	AStoryManager* StoryManager = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UDialogs* Dialogs = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	ALCharacter* Char = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UStory* Story = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	int32 ChapterId = 1; // Maybe this could be on the game instance. ?
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FLChapter Chapter;

protected:
	void StartStory();
	UFUNCTION() // bind to delegate
	void StartNextChapter();
	UFUNCTION() // bind to delegate
	void DiagShown(const FDialog& Diag);
	UFUNCTION() // bind to delegate
	void DiagDone();
	bool CharInputEnabled = true;
};
