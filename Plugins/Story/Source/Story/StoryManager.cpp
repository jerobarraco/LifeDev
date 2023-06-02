// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "StoryManager.h"

#include "Story.h"
#include "StoryUI.h"

AStoryManager::AStoryManager():Super() {
	static ConstructorHelpers::FClassFinder<UStoryUI> CUIClass(TEXT("/Story/UI/W_StoryUI"));
	UIClass = CUIClass.Succeeded() ? CUIClass.Class.Get() : UStoryUI::StaticClass();
}

void AStoryManager::Init_Implementation() {
	
}

void AStoryManager::DeInit_Implementation() {
	UI = nullptr;
}

void AStoryManager::FadeIn(const FText& Title, const FText& Text) {
	if (!IsValid(UI)) return;
	UI->FadeIn(Title, Text);
}

void AStoryManager::FadeOut() {
	if (!IsValid(UI)) return;
	UI->FadeOut();
}

void AStoryManager::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(UIClass.Get())) {
		UI = NewObject<UStoryUI>(this, UIClass, TEXT("StoryUI"));
		if (IsValid(UI)) {
			UI->AddToViewport();
		}
	}
}

void AStoryManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	DeInit();
}

