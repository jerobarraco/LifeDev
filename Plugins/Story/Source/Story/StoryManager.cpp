// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "StoryManager.h"

#include "Story.h"
#include "StoryUI.h"

AStoryManager::AStoryManager():Super() {
	static ConstructorHelpers::FClassFinder<UStoryUI> CUIClass(TEXT("/Story/UI/W_StoryUI"));
	UIClass = CUIClass.Succeeded() ? CUIClass.Class.Get() : UStoryUI::StaticClass();
}

void AStoryManager::Init_Implementation() {}

void AStoryManager::DeInit_Implementation() {
	if (LIKELY(IsValid(Story)))
		Story->OnFade.RemoveAll(this);
	Story = nullptr;

	if (LIKELY(UI))
		UI->RemoveFromParent();
	UI = nullptr;
}

void AStoryManager::Fade(const bool In, const FText& Title) {
	UE_LOG(LogTemp, Log, TEXT("Fading in=%i title='%s'"), In, *Title.ToString());
	if (In) {
		FadeIn();
	} else {
		FadeOut(Title);
	}
}

void AStoryManager::FadeIn() {
	if (UNLIKELY(!IsValid(UI))) return;
	// important to re-set the fade time
	UI->AnimDuration = Story->FadeTime;
	UI->FadeIn();
}

void AStoryManager::FadeOut(const FText& Title, const FText& Text) {
	if (UNLIKELY(!IsValid(UI))) return;
	// important to re-set the fade time
	UI->AnimDuration = Story->FadeTime;
	UI->FadeOut(Title, Text);
}

void AStoryManager::ShowBGSolid(const bool Show) const {
	if (LIKELY(!IsValid(UI))) return;
	UI->ShowBGSolid(Show);
}

void AStoryManager::UIFaded() {
	OnFaded.Broadcast();
}

void AStoryManager::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	
	Story = World->GetSubsystem<UStory>();
	if (LIKELY(Story)) Story->OnFade.AddUniqueDynamic(this, &AStoryManager::Fade);
	
	if (LIKELY(IsValid(UIClass.Get()))) {
		UI = CreateWidget<UStoryUI>(World, UIClass, TEXT("StoryUI"));
		if (LIKELY(IsValid(UI))) {
			UI->AddToViewport(ZOrder);
			UI->OnDone.AddUniqueDynamic(this, &AStoryManager::UIFaded);
		}
	}
}

void AStoryManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DeInit();
	Super::EndPlay(EndPlayReason);
}

