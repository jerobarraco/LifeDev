// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "GameUI.h"

#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/Flags.h"
#include "LifeDev/Core/Consts/ConstFlags.h"
#include "Story/Step.h"
#include "Story/Story.h"

void UGameUI::SetPrompt_Implementation(const FText& Text) {
	if (UNLIKELY(!T_Prompt)) return;
	T_Prompt->SetText(Text);
}

void UGameUI::PromptShow_Implementation(const FText& Text) {
	if (UNLIKELY(!T_Prompt)) return;
	T_Prompt->SetText(Text);
	if (LIKELY(PromptBG)) PromptBG->SetVisibility(ESlateVisibility::Visible);
}

void UGameUI::PromptHide_Implementation() {
	if (LIKELY(PromptBG)) PromptBG->SetVisibility(ESlateVisibility::Collapsed);
}

void UGameUI::SetPointerShow_Implementation(const bool Vis) {
	if (LIKELY(Pointer)) Pointer->SetVisibility(Vis ? ESlateVisibility::Visible: ESlateVisibility::Hidden);
}

void UGameUI::Init() {
	
	ALLearnMan* const LearnMan = ALLearnMan::Instance(this);
	
	if (LIKELY(!!LearnMan & !!UI)) { // this should be somewhere else tbh.
		LearnMan->OnShow.AddUniqueDynamic(UI, UGameUI::LearnShow);
		LearnMan->OnHide.AddUniqueDynamic(UI, UGameUI::LearnHide);
	}
}

void UGameUI::DeInit() {
	
	ALLearnMan* const LearnMan = ALLearnMan::Instance(this);
	
	if (LIKELY(!!LearnMan & !!UI)) { // this should be somewhere else tbh.
		LearnMan->OnShow.AddUniqueDynamic(UI, UGameUI::LearnShow);
		LearnMan->OnHide.AddUniqueDynamic(UI, UGameUI::LearnHide);
	}
}

void UGameUI::ShowStatus(const bool Enabled) const {
	if (LIKELY(GStatus)) GStatus->SetVisibility(Enabled ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
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
	if (LIKELY(IsValid(TTime)) && Name == LDConsts::Flags::Stats::Global::Time) {
		static FText TTimeF = NSLOCTEXT("Status", "Time", "Time {0}s");
		// FFormatOrderedArguments Args;
		// Args.Add(FText::AsNumberTotal);
		TTime->SetText(FText::FormatOrdered(TTimeF, FText::AsNumber(Total)));
	}
}

void UGameUI::StepStart(AStep* const Step) {
	if (LIKELY(Step && IsValid(TState))) {
		static FText TStateF = NSLOCTEXT("Status", "State", "State '{0}'");
		// FFormatOrderedArguments Args;
		// Args.Add(Step->Name.ToString());
		TState->SetText(FText::FormatOrdered(TStateF, FText::FromString(Step->Name.ToString())));
	}
}
