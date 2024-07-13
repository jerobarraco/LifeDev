// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "LSysSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LSettings.generated.h"

class USaveGame;
class ULSave;
class ULGUSettings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeatUpdate, const EFeat, Feat, const bool, Enabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveReady);

// This class allows to interact with the in-game settings during gameplay.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSettings: public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ULSettings* Instance(UObject* O);
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static bool GetFeatS(UObject* O, EFeat Feat);

	// overrides current save with a new game
	// defaults to current slot
	UFUNCTION(BlueprintCallable)
	void NewGame(int32 SlotIndex =-1);
	// loads a game, or creates a new one if not found.
	// Defaults to current slot.
	UFUNCTION(BlueprintCallable)
	void LoadGame(int32 SlotIndex =-1);
	// saves the current game if any
	// defaults to current slot
	UFUNCTION(BlueprintCallable)
	void SaveGame(int32 SlotIndex =-1);
	// returns true if it's busy saving/loading
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsSaving() { return IsSaving; }
	// returns the current slot
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetSlotIndex() { return SlotIndex; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 CurrentChapter() const;

	// Reset all features to factory default (depends on if it's a shipping build)
	UFUNCTION(BlueprintCallable)
	void ResetFeats();

	// Sets a feature to be enabled or disabled
	UFUNCTION(BlueprintCallable)
	void SetFeat(EFeat Feat, bool Enable = true);

	// Returns true if a feature is enabled
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE	bool GetFeat(EFeat Feat) const { return Feats.Contains(Feat); }
	
	// only call once by the gameinstance or smth
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
	void Init();

	// the current save-game object. can be null.
	UPROPERTY(BlueprintReadOnly, Transient)
	ULSave* Save = nullptr;

	// triggered after save or load. even on error.
	UPROPERTY(BlueprintAssignable, Transient)
	FOnSaveReady OnSaveReady;
	
	// triggered when one of the feat is updated to be enabled or disabled
	UPROPERTY(BlueprintAssignable, Transient)
	FOnFeatUpdate OnFeatUpdate;
	UPROPERTY(BlueprintAssignable, Transient)
	FOnFeatUpdate OnFeatUpdateChap;
	UPROPERTY(BlueprintAssignable, Transient)
	FOnFeatUpdate OnFeatUpdateDiags;
	UPROPERTY(BlueprintAssignable, Transient)
	FOnFeatUpdate OnFeatUpdateSound;
	UPROPERTY(BlueprintAssignable, Transient)
	FOnFeatUpdate OnFeatUpdateVisual;
	UPROPERTY(BlueprintAssignable, Transient)
	FOnFeatUpdate OnFeatUpdateAccess;
	UPROPERTY(BlueprintAssignable, Transient)
	FOnFeatUpdate OnFeatUpdateDebug;

protected:
	// broadcasts the corresponding delegates
	void FeatUpdated(EFeat Feat, bool Enable) const;
	void SaveGameDone(const FString& Slot, int32 Index, bool Success);
	void LoadGameDone(const FString& Slot, int32 Index, USaveGame* LoadedGame);

	// the current feats enabled in the game
	UPROPERTY(BlueprintReadOnly, Transient)
	TSet<EFeat> Feats;
	
	bool IsSaving = false;
	int32 SlotIndex = 0;
};
