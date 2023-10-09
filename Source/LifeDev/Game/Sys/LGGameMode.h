// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LifeDev/Core/Settings/FLChapter.h"

#include "LGGameMode.generated.h"

class UInputMappingContext;
class ALDialogMan;
class ALInventoryManager;
class AStoryManager;
class ULSysSettings;
class ALChar;
class UStory;
class UDialogs;
class ALMusicMan;
class AFlashbackMan;
class UInventory;
class UFlags;

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

	// not working properly atm
	UFUNCTION(BlueprintCallable, meta=(DeprecatedFunction))
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

	UFUNCTION(BlueprintCallable)
	void Fade(const FText& DecoratedTitle = INVTEXT(""));
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	ALDialogMan* DiagManager = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UDialogs* Dialogs = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	ALInventoryManager* InvManager = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UInventory* Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UFlags* Flags = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	AStoryManager* StoryManager = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	UStory* Story = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	ALChar* Char = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	ALMusicMan* MusicMan = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	AFlashbackMan* FlashbackMan = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	int32 ChapterId = 1; // Maybe this could be on the game instance. ?
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FLChapter Chapter;
	// TODO deprecated, should read the variable from the Story subs
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DeprecatedProperty))
	float TimeFadeIn = 2;
	// TODO deprecated, should read the variable from the Story subs
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DeprecatedProperty))
	float TimeHold = 2;

	// not supported on desktop and could break the game
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool UseDynRes = false;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostLoad() override;

	void StartChapter();
	UFUNCTION() // bind to delegate
	void StartNextChapter();
	UFUNCTION() // bind to delegate
	void DiagShown(const FDialog& Diag);
	UFUNCTION() // bind to delegate
	void DiagDone();
	
	void SetDynRes();

	bool CharInputEnabled = true;
};
