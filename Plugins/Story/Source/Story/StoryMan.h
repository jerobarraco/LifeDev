// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CoreMinimal.h"

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

	UFUNCTION(Blueprintable)
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
	// called when the ui is done fading
	UFUNCTION()
	void UIFaded();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UStoryUI> UI = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UStory> Story = nullptr;
};
