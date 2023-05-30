// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Subsystems/WorldSubsystem.h"

#include "Story.generated.h"

class AStep;
class UDataTable;
// World subsystem to deal with Inventory
UCLASS(Blueprintable, Category="Inventory")
class STORY_API UStory : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init();
	virtual void Init_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeInit();
	virtual void DeInit_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Start(const FName& Name);
	virtual void Start_Implementation(const FName& Name);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Stop(const FName& WithName=FName(""));
	virtual void Stop_Implementation(const FName& WithName=FName(""));
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient)
	TArray<AStep*> Steps;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	AStep* Current = nullptr;
protected:

};
