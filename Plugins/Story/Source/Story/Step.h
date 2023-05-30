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
	AStep();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Start() ;
	virtual void Start_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Stop();
	virtual void Stop_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void Finish();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	AActor* CamTarget = nullptr;
};
