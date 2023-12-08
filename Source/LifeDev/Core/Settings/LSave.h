// Copyright (C) 2023 Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LSave.generated.h"

// baseclass for lifedev savegame
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSave : public USaveGame {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Reset();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ChapterID = -1;
};
