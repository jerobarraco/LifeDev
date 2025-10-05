// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "GameUI.h"

#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"

#include "Inventory/Flags.h"
#include "Story/Step.h"
#include "Story/Story.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Game/Inventory/Teach/LTeachMan.h"

void UGameUI::PromptSet_Implementation(const FText& Text) {
	if (UNLIKELY(!TPrompt)) return;
	TPrompt->SetText(Text);
}

void UGameUI::PromptShow_Implementation(const FText& Text) {
	PromptSet(Text);
	if (LIKELY(APromptIn))
		PlayAnimation(APromptIn, 0, 1, EUMGSequencePlayMode::Forward, AnimSpeed);
	else if (LIKELY(PromptBG))
		PromptBG->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UGameUI::PromptHide_Implementation() {
	if (LIKELY(APromptIn))
		PlayAnimation(APromptIn, 0, 1, EUMGSequencePlayMode::Reverse, AnimSpeed);
	else if (LIKELY(PromptBG))
		PromptBG->SetVisibility(ESlateVisibility::Hidden);
}

void UGameUI::SetPointerShow_Implementation(const bool Vis) {
	if (LIKELY(Pointer)) Pointer->SetVisibility(Vis ? ESlateVisibility::HitTestInvisible: ESlateVisibility::Hidden);
}

void UGameUI::Init() {
	ALTeachMan* const TeachMan = ALTeachMan::Instance(this);
	if (LIKELY(TeachMan)) { // this should be somewhere else tbh.
		TeachMan->OnShow.AddUniqueDynamic(this, &UGameUI::LearnShow);
		TeachMan->OnHide.AddUniqueDynamic(this, &UGameUI::LearnHide);
	}

	// patch. avoid issue with the game starting by hiding the prompt and hence showing the animation unnecessarily
	TPrompt->SetText(FText::GetEmpty());
	if (LIKELY(TTeach)) TTeach->SetText(FText::GetEmpty());
}

void UGameUI::DeInit() {
	ALTeachMan* const TeachMan = ALTeachMan::Instance(this);
	if (LIKELY(TeachMan)) { // this should be somewhere else tbh.
		TeachMan->OnShow.RemoveAll(this);
		TeachMan->OnHide.RemoveAll(this);
	}
}

void UGameUI::LearnShow(const FName& Id, const FTeachRow& Row) {
	UE_LOG(LogTemp, Log, TEXT("UI:%hs Id=%s"), __func__, *Id.ToString());

	if (UNLIKELY(!TTeach)) return;

	TTeach->SetText(Row.Text);
	if (LIKELY(ATeachIn))
		PlayAnimation(ATeachIn, 0, 1, EUMGSequencePlayMode::Forward, AnimSpeed);
	else if (LIKELY(TeachBG))
		TeachBG->SetVisibility(ESlateVisibility::HitTestInvisible); // controlled by animation
}

void UGameUI::LearnHide(const FName& Id) {
	UE_LOG(LogTemp, Log, TEXT("UI:%hs Id=%s"), __func__, *Id.ToString());
	if (LIKELY(ATeachIn))
		PlayAnimation(ATeachIn, 0, 1, EUMGSequencePlayMode::Reverse, AnimSpeed);
	else if (LIKELY(TeachBG))
		TeachBG->SetVisibility(ESlateVisibility::Hidden); // controlled by animation
}

void UGameUI::ShowStatus(const bool Enabled) const {
	if (LIKELY(GStatus)) GStatus->SetVisibility(Enabled ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	UFlags* const Flags = UFlags::Instance(this);
	UStory* const Story = UStory::Instance(this);
	if (Enabled) {
		if (LIKELY(Flags)) Flags->OnMod.AddUniqueDynamic(this, &UGameUI::FlagMod);
		if (LIKELY(Story)) Story->OnStart.AddUniqueDynamic(this, &UGameUI::StepStart);
	} else {
		if (LIKELY(Flags)) Flags->OnMod.RemoveAll(this);
		if (LIKELY(Story)) Story->OnStart.RemoveAll(this);
	}
}

void UGameUI::FlagMod(const FName& Name, const float Diff, const float Total) {
	if (LIKELY(IsValid(TTime)) & Name == LDConsts::Flags::Stats::Global::Time) {
		static FText TTimeF = NSLOCTEXT("Status", "Time", "Time {0}s");
		// FFormatOrderedArguments Args;
		// Args.Add(FText::AsNumberTotal);
		TTime->SetText(FText::FormatOrdered(TTimeF, FText::AsNumber(Total)));
	}
}

void UGameUI::StepStart(AStep* const Step) {
	if (LIKELY(bool(Step) & IsValid(TState))) {
		static FText TStateF = NSLOCTEXT("Status", "State", "State '{0}'");
		// FFormatOrderedArguments Args;
		// Args.Add(Step->Name.ToString());
		TState->SetText(FText::FormatOrdered(TStateF, FText::FromString(Step->Label.ToString())));
	}
}
