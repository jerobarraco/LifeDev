// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetDbgUI.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "Inventory/Flags.h"

#include "LifeDev/Core/Settings/LSettings.h"
// this ui is a waste of time. TODO rethink life choices.
// most of the time i would want to:
// * use the console
// * print logs
// so in the need to debug stuff i just need to make a debug build.
// thought there would be cases where i'd want to be able to debug certain stuff on a build, not on the editor, on an expo, on a maybe shipping build (for performance)
// though maybe i just use a debug build.

void ULSetDbgUI::Load_Implementation() {
	Super::Load_Implementation();
	// this is temporarily unhooked from lsettingsui for being useless.
	
	const bool Dbg = ULSettings::GetFeatS(this, EFeat::DBG_BASE);
	if (LIKELY(!Dbg)) return;

	if (UNLIKELY(!SBFlags)) return;

	SBFlags->ClearChildren();

	const UFlags* const Flags = UFlags::Instance(this);
	if (UNLIKELY(!Flags)) return;

	TMap<FName, float> Map;
	Map = Flags->GetAll();
	
	for (const TTuple<FName, float> KV : Map) {
		// ue says : CreateWidget called with a null class...
		UTextBlock* const Text = Cast<UTextBlock>(
			CreateWidget(this, UTextBlock::StaticClass()));
		if (UNLIKELY(!Text)) continue;
		SBFlags->AddChild(Text);
		Text->SetText(FText::FromString(
			FString::Printf(TEXT("%s: %.4f"), *KV.Key.ToString(), KV.Value)));
	}
}
