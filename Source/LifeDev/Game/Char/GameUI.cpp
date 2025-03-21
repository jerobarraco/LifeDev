// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "GameUI.h"

#include "Components/TextBlock.h"

void UGameUI::SetPrompt_Implementation(const FText& Text) {
	if (!T_Prompt) return;
	T_Prompt->SetText(Text);
}

void UGameUI::PromptShow_Implementation(const FText& Text) {}
void UGameUI::PromptHide_Implementation() {}
void UGameUI::SetPointerVisible_Implementation(const bool Vis) {}
