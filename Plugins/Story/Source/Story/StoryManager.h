// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#pragma once
#include "CoreMinimal.h"

#include "StoryManager.generated.h"

class AStep;
class UStoryUI;

UCLASS(Blueprintable, BlueprintType)
class STORY_API AStoryManager: public AActor {
	GENERATED_BODY()

public:

	AStoryManager();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init();
	virtual void Init_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DeInit();
	virtual void DeInit_Implementation();
	
	UFUNCTION(BlueprintCallable)
	void FadeIn(const FText& Title = FText::GetEmpty(), const FText& Text = FText::GetEmpty());

	UFUNCTION(BlueprintCallable)
	void FadeOut();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UStoryUI> UIClass = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	UStoryUI* UI = nullptr;
};
