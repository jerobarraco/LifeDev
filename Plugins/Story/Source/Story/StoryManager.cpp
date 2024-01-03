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
	if (IsValid(Story)) {
		Story->OnFade.RemoveAll(this);
	}
	Story = nullptr;

	if (UI) {
		UI->RemoveFromParent();
	}
	UI = nullptr;
}

void AStoryManager::Fade(bool In, const FText& Title) {
	UE_LOG(LogTemp, Log, TEXT("Fading in=%i text='%s'"), In, *Title.ToString());
	if (In) {
		FadeIn(Title);
	} else {
		FadeOut();
	}
}

void AStoryManager::FadeIn(const FText& Title, const FText& Text) {
	if (!IsValid(UI)) return;
	UI->FadeIn(Title, Text);
}

void AStoryManager::FadeOut() {
	if (!IsValid(UI)) return;
	UI->FadeOut();
}

void AStoryManager::UIFaded() {
	OnFaded.Broadcast();
}

void AStoryManager::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (!World) return;
	
	Story = World->GetSubsystem<UStory>();
	if (Story) {
		Story->OnFade.AddUniqueDynamic(this, &AStoryManager::Fade);
	}
	
	if (IsValid(UIClass.Get())) {
		UI = CreateWidget<UStoryUI>(World, UIClass, TEXT("StoryUI"));
		if (IsValid(UI)) {
			UI->AddToViewport(ZOrder);
			UI->OnDone.AddUniqueDynamic(this, &AStoryManager::UIFaded);
		}
	}
}

void AStoryManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DeInit();
	Super::EndPlay(EndPlayReason);
}

