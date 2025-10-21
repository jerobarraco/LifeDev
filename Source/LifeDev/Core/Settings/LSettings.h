// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "LifeDev/Core/Consts/ConstSettings.h"

#include "LSettings.generated.h"

class USaveGame;
class ULSave;
class ULGUSettings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFeatUpdate, const EFeat, Feat, const bool, Enabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaving, const bool, IsSaving);

// This class allows to interact with the in-game settings during gameplay.
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSettings: public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static ULSettings* Instance(const UObject* const O);
	UFUNCTION(BlueprintCallable, meta=(WorldContext="O"))
	static bool GetFeatS(const UObject* const O, const EFeat Feat);
	UFUNCTION(BlueprintCallable)
	static FName GetObjectLabel(const UObject* const Object);

	// overrides current save with a new game
	// defaults to current slot
	// the object is ready after the call, but it won't be saved just yet.
	UFUNCTION(BlueprintCallable)
	void NewGame(const int32 NewSlotIndex =-1);
	
	// loads a game, or creates a new one if not found.
	// Defaults to current slot (-1).
	UFUNCTION(BlueprintCallable)
	void LoadGame(const int32 NewSlotIndex =-1);
	// saves the current game if any
	// defaults to current slot (-1)
	UFUNCTION(BlueprintCallable)
	void SaveGame(const int32 NewSlotIndex =-1);
	// returns true if it's busy saving/loading
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetIsSaving() const { return IsSaving; }
	// returns the current slot
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetSlotIndex() const { return SlotIndex; }

	// returns the current chapter id. -1 if no valid save or chapter is loaded.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 CurrentChapter() const;

	// returns the current chapter feature.
	// or C_NONE on chapid <0, or C_DONE if over the max.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EFeat CurrentChapterFeat() const;
	
	// Reset all features to factory default (depends on if it's a shipping build)
	UFUNCTION(BlueprintCallable)
	void ResetFeats();

	// Sets a feature to be enabled or disabled
	UFUNCTION(BlueprintCallable)
	void SetFeat(const EFeat Feat, const bool Enable = true);

	// Returns true if a feature is enabled
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE	bool GetFeat(const EFeat Feat) const { return Feats.Contains(Feat); }
	
	// only call once by the gameinstance or smth
	UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
	void Init();

	// the current save-game object. can be null.
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<ULSave> Save = nullptr;

	// triggered after save or load. even on error.
	UPROPERTY(BlueprintAssignable, Transient)
	FOnSaving OnSaving;
	
	// triggered when one of the feat is updated to be enabled or disabled.
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
	FOnFeatUpdate OnFeatUpdateGameplay;
	UPROPERTY(BlueprintAssignable, Transient)
	FOnFeatUpdate OnFeatUpdateAccess;
	UPROPERTY(BlueprintAssignable, Transient)
	FOnFeatUpdate OnFeatUpdateDebug;
	UPROPERTY(BlueprintAssignable, Transient)
	FOnFeatUpdate OnFeatUpdateUnreal;

protected:
	// broadcasts the corresponding delegates
	void FeatUpdated(const EFeat Feat, const bool Enable) const;
	void SaveGameDone(const FString& Slot, const int32 Index, const bool Success);
	void LoadGameDone(const FString& Slot, const int32 Index, USaveGame* const LoadedGame);
	void SetIsSaving(bool NewIsSaving);

	// the current feats enabled in the game
	UPROPERTY(BlueprintReadOnly, Transient)
	TSet<EFeat> Feats;

	bool IsSaving = false;
	int32 SlotIndex = 0;
};
