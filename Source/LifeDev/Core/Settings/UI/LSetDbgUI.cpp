// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetDbgUI.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "Inventory/Flags.h"

#include "LifeDev/Core/Settings/LSettings.h"

void ULSetDbgUI::Load_Implementation() {
	Super::Load_Implementation();
	const bool Dbg = ULSettings::GetFeatS(this, EFeat::DBG_BASE);
	if (LIKELY(!Dbg)) return;

	if (UNLIKELY(!SBFlags)) return;

	SBFlags->ClearChildren();

	const UFlags* const Flags = UFlags::Instance(this);
	if (UNLIKELY(!Flags)) return;

	TMap<FName, float> Map;
	Map = Flags->GetAll();
	for (const TTuple<FName, float> KV : Map) {
		UTextBlock* const Text = Cast<UTextBlock>(CreateWidget(this, UTextBlock::StaticClass()));
		if (!Text) continue;
		SBFlags->AddChild(Text);
		Text->SetText(FText::FromString(
			FString::Printf(TEXT("%s: %.4f"), *KV.Key.ToString(), KV.Value)
			));
	}
}
