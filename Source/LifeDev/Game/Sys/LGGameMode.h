// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LifeDev/Core/Settings/FLChapter.h"

#include "LGGameMode.generated.h"

class ALStoryMan;
struct FDialog;
class APostProcessVolume;
class ULSettings;
class UInputMappingContext;
class ALDialogMan;
class ALInventoryManager;
class ALFeatsMan;
class ALMusicMan;
class AFlashbackMan;
class AStoryManager;
class ULSysSettings;
class ALChar;
class UStory;
class UDiags;
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

	// not working properly atm
	UFUNCTION(BlueprintCallable, meta=(DeprecatedFunction, AdvancedDisplay))
	static ALGGameMode* Instance(UWorld* World = nullptr);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	virtual void Init_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeInit();
	virtual void DeInit_Implementation();

	// enables or disables char input. and stays like that even though of Diags.
	UFUNCTION(BlueprintCallable)
	void SetCharInputEnabled(bool Enabled);

	// wraps SetTempInputEnabled. used for delegates
	UFUNCTION() // bind to delegate
	void SetInputEnable() { SetTempInputEnabled(true); }
	// wraps SetTempInputEnabled. used for delegates
	UFUNCTION() // bind to delegate
	void SetInputDisable() { SetTempInputEnabled(false); }
	
	// disables input temporarily, enables only if char is enabled.
	// used for Diags and temporary things.
	UFUNCTION(BlueprintCallable)
	void SetTempInputEnabled(bool Enabled);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	FLChapter Chapter;

	/// cache: these are public on purpose in case someone needs a handy dandy pointer to them
	/// only making public the ones that require a GetActorOfClass (not the subsystems)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	ALDialogMan* DiagMan = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	ALInventoryManager* InventoryMan = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	ALStoryMan* StoryMan = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	ALMusicMan* MusicMan = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	AFlashbackMan* FlashbackMan = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	ALFeatsMan* FeatsMan = nullptr;

	/// others cache
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	ALChar* Char = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	APostProcessVolume* PostProcess = nullptr;

	/// subsystems cache
	UPROPERTY(BlueprintReadOnly, Transient)
	UDiags* Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UInventory* Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UFlags* Flags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	UStory* Story = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	ULSettings* Settings = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool LoadChapter();
	void StartChapter();

	UFUNCTION() // bind to delegate
	void StartNextChapter();
	UFUNCTION() // bind to delegate
	void DiagShown(const FDialog& Diag);
	UFUNCTION() // bind to delegate
	void DiagDone();
	UFUNCTION() // bound
	void Fade(bool bIn, const FText& Text);

	bool CharInputEnabled = true;
	
	// not supported on desktop and could break the game
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool UseDynRes = false;
};
