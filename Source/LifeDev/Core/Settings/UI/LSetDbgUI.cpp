// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetDbgUI.h"

#include "LFeatsGroup.h"
#include "Components/ScrollBox.h"

#include "LifeDev/Core/Settings/LSettings.h"
// this ui is a waste of time. TODO rethink life choices.
// most of the time i would want to:
// * use the console
// * print logs
// so in the need to debug stuff i just need to make a debug build.
// thought there would be cases where i'd want to be able to debug certain stuff on a build, not on the editor, on an expo, on a maybe shipping build (for performance)
// though maybe i just use a debug build.

void ULSetDbgUI::Apply_Implementation() {
	Super::Apply_Implementation();
	
	if (LIKELY(FeatsDbg))
		FeatsDbg->Apply();
}

void ULSetDbgUI::Load_Implementation() {
	Super::Load_Implementation();
	// this is temporarily unhooked from lsettingsui for being useless.
	
	const bool Dbg = ULSettings::GetFeatS(this, EFeat::DBG_BASE);
	if (LIKELY(!Dbg)) return;

	if (LIKELY(FeatsDbg))
		FeatsDbg->Load();

	if (UNLIKELY(!SBFlags)) return;

	SBFlags->ClearChildren();

	// const UFlags* const Flags = UFlags::Instance(this);
	// if (UNLIKELY(!Flags)) return;

	// TMap<FName, float> Map;
	// Map = Flags->GetAll();

		
	// for (const TTuple<FName, float> KV : Map) {
		// ue says : CreateWidget called with a null class...
		// can't call CreateWidget with UTextBlock, it's not a UUSerWidget, but a UWidget :'(
		// UTextBlock* const Text = Cast<UTextBlock>(
			// CreateWidget(this, UTextBlock::StaticClass()));
		// if (UNLIKELY(!Text)) continue;
		// SBFlags->AddChild(Text);
		// Text->SetText(FText::FromString(
			// FString::Printf(TEXT("%s: %.4f"), *KV.Key.ToString(), KV.Value)));
	// }
}

void ULSetDbgUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	
	if (LIKELY(FeatsDbg)) {
		FeatsDbg->SetUp({
			// {EFeat::G_SHOW_POINT, FText::FromString("G_SHOW_POINT")}, // already in setgameui
			{EFeat::G_SHOW_DIAG, FText::FromString("G_SHOW_DIAG")},
			{EFeat::G_SHOW_INV, FText::FromString("G_SHOW_INV")},
			{EFeat::G_FLY_CAM, FText::FromString("G_FLY_CAM")},
			{EFeat::DBG_STEPS, FText::FromString("DBG_STEPS")},
			{EFeat::DBG_ANIMS, FText::FromString("DBG_ANIMS")},
			{EFeat::DBG_SOUND, FText::FromString("DBG_SOUND")},
			{EFeat::DBG_FB, FText::FromString("DBG_FB")},
			{EFeat::DBG_SIG, FText::FromString("DBG_SIG")},
			{EFeat::DBG_TESTDL, FText::FromString("DBG_TESTDL")}
		});
	}
}
