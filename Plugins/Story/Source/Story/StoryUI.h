// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "Blueprint/UserWidget.h"

#include "StoryUI.generated.h"

class UImage;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoryUIDone);

// The ui used for fade in/out
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STORY_API UStoryUI: public UUserWidget {
public:
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FadeIn();
	void FadeIn_Implementation(){};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FadeOut(const FText& Title = FText::GetEmpty(), const FText& Text = FText::GetEmpty());
	void FadeOut_Implementation(const FText& Title, const FText& Text) {}
	
	UFUNCTION(BlueprintCallable)
	void ShowBGSolid(const bool Show = true);
	
	// variable that indicates when the Story is ready to progress.
	// false when animating.
	UPROPERTY(BlueprintReadWrite)
	bool IsShowing = false;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetAnimSpeed() const { return AnimDuration > 0 ? 1.0f/AnimDuration: 1.0f;}

	// How long fading in/out should take
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float AnimDuration = 2.f;

	// the manager will handle this.
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FStoryUIDone OnDone;

protected:
	UFUNCTION(BlueprintCallable)
	void AnimFinished();
	
	UFUNCTION(BlueprintCallable)
	void PostHide();

	UFUNCTION(BlueprintCallable)
	void PreShow();

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UImage> BGSolid = nullptr;
};
