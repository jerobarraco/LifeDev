// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "GameFramework/Actor.h"

#include "StoryMan.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoryManagerOnFaded);

class AStep;
class UStoryUI;
class UStory;

UCLASS(Blueprintable, BlueprintType)
class STORY_API AStoryMan: public AActor {
	GENERATED_BODY()

public:
	AStoryMan();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void Init();
	virtual void Init_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta=(ForceAsFunction))
	void DeInit();
	virtual void DeInit_Implementation();

	UFUNCTION(Blueprintable, meta=(AutoCreateRefTerm=Title))
	void Fade(bool In, const FText& Title = FText::GetEmpty());
	
	UFUNCTION(BlueprintCallable)
	void FadeIn();

	UFUNCTION(BlueprintCallable)
	void FadeOut(const FText& Title = FText::GetEmpty(), const FText& Text = FText::GetEmpty());

	// shows a solid bg. used as a way to fade the whole screen during level loading.
	UFUNCTION(BlueprintCallable)
	void ShowBGSolid(const bool Show) const;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	int32 ZOrder = 5;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	TSubclassOf<UStoryUI> UIClass = nullptr;

	// Triggered once the ui has completely faded
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="SetUp")
	FStoryManagerOnFaded OnFaded;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// called when the ui is done fading
	UFUNCTION()
	void UIFaded();

	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void StepStart(AStep* Step);

	UFUNCTION(BlueprintNativeEvent, meta=(ForceAsFunction))
	void StepStop(AStep* Step); // can't use const due to native event
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UStoryUI> UI = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UStory> Story = nullptr;
};
