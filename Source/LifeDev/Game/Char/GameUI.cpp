// Copyright (C) 2023 - Jerónimo Barraco-Mármol

#include "GameUI.h"

#include "Components/TextBlock.h"

void UGameUI::SetPrompt_Implementation(const FText& Text) {
	if (!T_Prompt) return;
	T_Prompt->SetText(Text);
}

void UGameUI::InteractShowPrompt_Implementation(const FText& Text) {}
void UGameUI::InteractHidePrompt_Implementation() {}
