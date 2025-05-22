// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "StoryMan.h"

#include "Story.h"
#include "StoryUI.h"

AStoryMan::AStoryMan():Super() {
	static ConstructorHelpers::FClassFinder<UStoryUI> CUIClass(TEXT("/Story/UI/W_StoryUI"));
	UIClass = LIKELY(CUIClass.Succeeded()) ? CUIClass.Class.Get() : UStoryUI::StaticClass();
}

void AStoryMan::Init_Implementation() {
	if (LIKELY(Story)) {
		Story->OnFade.AddUniqueDynamic(this, &AStoryMan::Fade);
		Story->OnStart.AddUniqueDynamic(this, &AStoryMan::StepStart);
		Story->OnStop.AddUniqueDynamic(this, &AStoryMan::StepStop);
	}
}

void AStoryMan::DeInit_Implementation() {
	if (LIKELY(IsValid(Story))) {
		Story->OnFade.RemoveAll(this);
		Story->OnStart.RemoveAll(this);
		Story->OnStop.RemoveAll(this);
	}
	Story = nullptr;

	if (LIKELY(UI))
		UI->RemoveFromParent();
	UI = nullptr;
}

void AStoryMan::Fade(const bool In, const FText& Title) {
	UE_LOG(LogTemp, Log, TEXT("Fading in=%i title='%s'"), In, *Title.ToString());
	if (In) {
		FadeIn();
	} else {
		FadeOut(Title);
	}
}

void AStoryMan::FadeIn() {
	if (UNLIKELY(!IsValid(UI))) return;
	// important to re-set the fade time
	UI->AnimDuration = Story->FadeTime;
	UI->FadeIn();
}

void AStoryMan::FadeOut(const FText& Title, const FText& Text) {
	if (UNLIKELY(!IsValid(UI))) return;
	// important to re-set the fade time
	UI->AnimDuration = Story->FadeTime;
	UI->FadeOut(Title, Text);
}

void AStoryMan::ShowBGSolid(const bool Show) const {
	if (LIKELY(!IsValid(UI))) return;
	UI->ShowBGSolid(Show);
}

void AStoryMan::UIFaded() {
	OnFaded.Broadcast();
}

void AStoryMan::BeginPlay() {
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	
	Story = World->GetSubsystem<UStory>();

	if (LIKELY(IsValid(UIClass.Get()))) {
		UI = CreateWidget<UStoryUI>(World, UIClass, TEXT("StoryUI"));
		if (LIKELY(IsValid(UI))) {
			UI->AddToViewport(ZOrder);
			UI->OnDone.AddUniqueDynamic(this, &AStoryMan::UIFaded);
		}
	}
}

void AStoryMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DeInit();
	Super::EndPlay(EndPlayReason);
}

void AStoryMan::StepStart_Implementation(AStep* const Step) {}
void AStoryMan::StepStop_Implementation(AStep* const Step) {}
