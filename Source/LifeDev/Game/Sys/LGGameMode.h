// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "LifeDev/Core/Settings/FLChapter.h"

#include "LGGameMode.generated.h"

class ALTeachMan;
class UEval;
class ALightPool;
class UFlashback;
struct FDiag;
class AGhostPool;
class ALStoryMan;
class APostProcessVolume;
class ULSettings;
class UInputMappingContext;
class ALDiagMan;
class ALInventoryMan;
class ALFeatsMan;
class ALMusicMan;
class AFlashbackMan;
class AStoryMan;
class ULSysSettings;
class ALChar;
class UStory;
class UDiags;
class UInventory;
class UFlags;

// to get the collision channel read https://gamedev.stackexchange.com/a/178812/44866
#define InteractTraceChannel ECC_GameTraceChannel1

// Lifedev main game mode. (for the game stage)
UCLASS(Blueprintable, Config=LifeDev, DefaultConfig)
class LIFEDEV_API ALGGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	ALGGameMode();

	// not working properly atm
	UFUNCTION(BlueprintCallable, meta=(DeprecatedFunction, AdvancedDisplay, WorldContext="O"))
	static ALGGameMode* Instance(const UObject* const O);

	// enables or disables char input. and stays like that even though of Diags.
	UFUNCTION(BlueprintCallable)
	void SetCharInputEnabled(const bool Enabled);

	// wraps SetTempInputEnabled. used for delegates
	UFUNCTION() // bind to delegate
	void SetInputEnable() { SetTempInputEnabled(true); }
	// wraps SetTempInputEnabled. used for delegates
	UFUNCTION() // bind to delegate
	void SetInputDisable() { SetTempInputEnabled(false); }
	
	// disables input temporarily, enables only if char is enabled.
	// used for Diags and temporary things.
	UFUNCTION(BlueprintCallable)
	void SetTempInputEnabled(const bool Enabled) const;

#pragma region Managers
	/// cache: these are public on purpose in case someone needs a handy dandy pointer to them
	/// only making public the ones that require a GetActorOfClass (not the subsystems)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<ALDiagMan> DiagMan = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<ALInventoryMan> InventoryMan = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<ALStoryMan> StoryMan = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<ALMusicMan> MusicMan = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<AFlashbackMan> FlashbackMan = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<ALFeatsMan> FeatsMan = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<ALTeachMan> TeachMan = nullptr;
#pragma endregion
#pragma region Cache
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<ALChar> Char = nullptr;
	// post process, the featsman needs it
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<APostProcessVolume> PostProcess = nullptr;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<AGhostPool> Ghosts = nullptr;
#pragma endregion
#pragma region Subsystems
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UDiags> Diags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UInventory> Inventory = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlags> Flags = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UStory> Story = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UFlashback> Flashback = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UEval> Eval = nullptr;
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ULSettings> Settings = nullptr;
#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Init();
	void DeInit();
	UFUNCTION()
	void InitOnSave(const bool IsSaving);
	void Spawn();
	
	UFUNCTION() // bind to delegate
	void DiagShown(const FDiag& Diag);
	UFUNCTION() // bind to delegate
	void DiagDone();
	UFUNCTION() // bound
	void Fade(const bool bIn, const FText& Text);

	void TickCounter() const;

	// how often to increase the time counting flag. beware if this is too small, it will flood the event for flag updated.
	// which some objects are subscribed to.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Config, Category="SetUp")
	float CounterTime=.5;

	/// transients

	bool CharInputEnabled = true;
	bool IsFirstFade = true;
	FTimerHandle CounterHandle;
};
