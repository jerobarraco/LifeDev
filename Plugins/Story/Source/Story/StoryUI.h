// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#pragma once
#include "Blueprint/UserWidget.h"

#include "StoryUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoryUIDone);

// The ui used for fade in/out
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STORY_API UStoryUI: public UUserWidget {
public:
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FadeIn(const FText& Title = FText::GetEmpty(), const FText& Text = FText::GetEmpty());

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FadeOut();
	
	// variable that indicates when the Story is ready to progress.
	// false when animating.
	UPROPERTY(BlueprintReadWrite)
	bool IsShowing = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=SetUp)
	float AnimSpeed = 1.0;

	// the manager will handle this.
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FStoryUIDone OnDone;

protected:

	UFUNCTION(BlueprintCallable)
	void AnimFinished();
	
	UFUNCTION(BlueprintCallable)
	void PostHide();

	UFUNCTION(BlueprintCallable)
	void PreShow();
};
