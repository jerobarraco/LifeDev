// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetDbgUI.h"

#include "JButton.h"
#include "LFeatsGroup.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Inventory/Flags.h"
#include "Kismet/KismetSystemLibrary.h"

#include "LifeDev/Core/Settings/LSettings.h"
// try not to rely on this ui.
// most of the time i would want to:
// * use the console commands
// * print logs
// so in the need to debug stuff i just need to make a debug build.
// thought there would be cases where i'd want to be able to debug certain stuff on a build,
// not on the editor, on an expo, on a maybe shipping build (for performance) (hence not testing for IsDebug here)
// though maybe i just use a debug build.

void ULSetDbgUI::Apply_Implementation() {
	Super::Apply_Implementation();
	
	const bool Dbg = ULSettings::GetFeatS(this, EFeat::DBG_BASE);
	if (LIKELY(!Dbg)) return;

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

	if (LIKELY(TFlags)) {
		FString T;
		const UFlags* const Flags = UFlags::Instance(this);
		if (LIKELY(Flags)) {
			TMap<FName, float> Map;
			Map = Flags->GetAll();
			for (const TTuple<FName, float> KV : Map) {
				T += FString::Printf(TEXT("%s \t %.4f\n"), *KV.Key.ToString(), KV.Value);
			}
		}
		TFlags->SetText(FText::FromString(T));
	}
}

void ULSetDbgUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	
	if (LIKELY(FeatsDbg)) {
		FeatsDbg->SetUp({
			// {EFeat::G_SHOW_POINT, FText::FromString("G_SHOW_POINT")}, // already in setgameui
				// todo change the ftext on the rest
			{EFeat::D_SHOW, UEnum::GetDisplayValueAsText(EFeat::D_SHOW)},
			{EFeat::G_SHOW_INV, FText::FromString("G_SHOW_INV")},
			{EFeat::DBG_FAST_COOL, FText::FromString("DBG_FAST_COOL")},
			{EFeat::DBG_FLY_CAM, FText::FromString("DBG_FLY_CAM")},
			{EFeat::DBG_D_WARN, FText::FromString("DBG_D_WARN")},
			{EFeat::DBG_STEPS, FText::FromString("DBG_STEPS")},
			{EFeat::DBG_ANIMS, FText::FromString("DBG_ANIMS")},
			{EFeat::DBG_SOUND, FText::FromString("DBG_SOUND")},
			{EFeat::DBG_FB, FText::FromString("DBG_FB")},
			{EFeat::DBG_SIG, FText::FromString("DBG_SIG")},
			{EFeat::DBG_TESTDL, FText::FromString("DBG_TESTDL")}
		});
	}

	if (LIKELY(BTrace))
		BTrace->OnClick.AddUniqueDynamic(this, &ULSetDbgUI::OnTrace);
}

void ULSetDbgUI::NativeDestruct() {
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "Trace.Stop");
	if (LIKELY(BTrace)) BTrace->OnClick.RemoveAll(this);

	Super::NativeDestruct();
}

void ULSetDbgUI::OnTrace(const int32 Id) {
	const bool Start = Id == 0;
	const TCHAR* const Cmd =
		Start ? TEXT("Trace.File TraceFile Gpu,Screenshot,Region,Bookmark,Frame,Cpu,Log,") : TEXT("Trace.Stop") ;
	BTrace->Id = !Id;
	BTrace->Label = FText::FromString(Start ? TEXT("Trace Stop") : TEXT("Trace Start"));
	BTrace->ResetStyle();
	UKismetSystemLibrary::ExecuteConsoleCommand(this, Cmd);
}
