// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#pragma once

#include "CoreMinimal.h"

#include "Step.generated.h"

class ACameraActor;

UCLASS(Blueprintable, BlueprintType)
class STORY_API AStep : public AActor {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Start() ;
	virtual void Start_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void Stop() ;
	virtual void Stop_Implementation();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName Name;
	
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	ACameraActor* Camera = nullptr;
};
