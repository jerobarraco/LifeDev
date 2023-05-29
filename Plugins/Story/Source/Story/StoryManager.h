// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#pragma once
#include "CoreMinimal.h"

#include "StoryManager.generated.h"

class AStep;

UCLASS(Blueprintable, BlueprintType)
class STORY_API AStoryManager: public AActor {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TMap<int32, TSubclassOf<AStep>> Steps;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init();
	virtual void Init_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeInit();
	virtual void DeInit_Implementation();
};
