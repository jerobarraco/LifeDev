// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
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
	
	// the current chapter.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ChapterID = -1;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FName, float> SFlags;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FName, int32> SInventory;
};

// TODO load inventory and save inventory. and flags
