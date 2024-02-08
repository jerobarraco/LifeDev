// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "LSysSettings.h"
#include "GameFramework/SaveGame.h"
#include "LSave.generated.h"

// baseclass for lifedev savegame
// this class can't access the world 
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSave : public USaveGame {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Reset();

	UFUNCTION(BlueprintCallable)
	void WriteSubsystems(UWorld* const W);
	
	UFUNCTION(BlueprintCallable)
	void ReadSubsystems(UWorld* const W);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 ItemsNum() const { return SInventory.Num(); }

	// the current chapter.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ChapterID = -1;

	// last save time
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int64 Time = 0;
	// current slot index used / to use.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 SlotIndex = 0;

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
		EFeat::V_BLUR, EFeat::V_LUMEN, EFeat::A_STROBE,
	};
};
