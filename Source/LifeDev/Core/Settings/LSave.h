// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "LSysSettings.h"

#include "LSave.generated.h"

// baseclass for lifedev savegame
// this class can't access the world 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSave : public USaveGame {
	GENERATED_BODY()

public:
	// resets the savestate. it will write to subsystems.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="W"))
	void Reset(UWorld* const W);

	// sets the subsystem data to what's on the savestate.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="W"))
	void WriteSubsystems(UWorld* const W);

	// sets the savestate data to what's on the subsystems.
	UFUNCTION(BlueprintCallable, meta=(WorldContext="W"))
	void ReadSubsystems(UWorld* const W);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 ItemsNum() const { return SInventory.Num(); }

	// this is just for internal use.
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(AdvancedDisplay))
	FORCEINLINE TMap<FName, float> GetFlags() { return SFlags; }

	// the current chapter.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ChapterID = -1;

	// last save time
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int64 Time = 0;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FName, float> SFlags;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FName, int32> SInventory;
	// saved feats
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TSet<EFeat> SFeats;

	// the features to affect during load/save, the rest will be ignored.
	inline static TSet<EFeat> WatchFeats = {
		EFeat::S_ENV, EFeat::S_MUSIC, EFeat::S_NOISE,
		EFeat::V_BLUR, EFeat::V_LUMEN, EFeat::V_STROBE,
		EFeat::V_FOV, EFeat::V_SPEED, EFeat::V_MLIGHTS,
		EFeat::V_FLASHBACK, EFeat::V_NANITE, EFeat::E_GHOSTPOOL
	};
};
