// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "GameUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

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
