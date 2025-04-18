// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSettingsUI.h"

#include "LSettings.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Interact/Animator/Anim.h"
#include "Inventory/Flags.h"
#include "JUtils/Misc/JUtilsMisc.h"
#include "JUtils/UI/GroupBox.h"
#include "Kismet/GameplayStatics.h"
#include "LifeDev/Core/Consts/ConstFlags.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"

namespace LSetUI {
	// first one should be skippable. needs to be in ascending order.
	constexpr float Scales[] = {1, .25, .5, .75, 1, 1.25, 1.5, 1.75, 2, 3, 4};
	constexpr uint8 ScalesLen = UJUtilsMisc::ArraySize(Scales);
}

ULSettingsUI::ULSettingsUI():Super() {
	// note: the intro level intro ui will set `ShowCursor` to false. to avoid having the mouse hidden.
	ShowCursor = true;
	AutoUnbind = false; // you only bind once.
}

namespace LDConsts { namespace Static {
	static const FName TimeFade = "TimeFade";
}}

void ULSettingsUI::Show_Implementation() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	// the order matters.

	Super::Show_Implementation(); // basically show

	// start fading

	const ALMusicMan* const Man = ALMusicMan::Instance(this);
	if (LIKELY(Man)) Man->FadeFX(true);

	UAnim* const Anim = UAnim::Instance(this);
	if (LIKELY(Anim)) {
		const FAParams P {.Duration = PauseTime, .UseDilation = false};
		Anim->TimeFade(this, P, .2);
	}

	// then load
	Load();

	// finally set timer.
	PauseTimer.Invalidate();
	World->GetTimerManager().SetTimer(PauseTimer, this, &ULSettingsUI::SetPause, PauseTime);
}


void ULSettingsUI::Hide_Implementation() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	// order matters
	World->GetTimerManager().ClearTimer(PauseTimer);
	PauseTimer.Invalidate();

	const ALMusicMan* const Man = ALMusicMan::Instance(this);
	if (LIKELY(Man)) Man->FadeFX(false);

	UAnim* const Anim = UAnim::Instance(this);
	if (LIKELY(Anim)) {
		const FAParams P {.Duration = PauseTime, .UseDilation = false};
		Anim->TimeFade(this, P, 1);
	}

	Super::Hide_Implementation();

	UGameplayStatics::SetGamePaused(this, false);
	// SetVisibility(ESlateVisibility::Collapsed);
}

void ULSettingsUI::Load_Implementation() {
	// doesn't work as expected and prints errors
	// if (LIKELY(Settings_Dbg)) Settings_Dbg->Load();

	// on load since this could also be on the intro level. so it could change on each show.
	if (LIKELY(TFoxy)) {
		const UFlags* const Flags = UFlags::Instance(this);
		const float Foxy = Flags ? Flags->Get(LDConsts::Flags::Settings::Global::Foxy) : 0;
		TFoxy->SetText(FText::FromString(FString::Printf(TEXT("Foxy: %.4f"), Foxy)));
		TFoxy->SetVisibility(ESlateVisibility::Visible);
	}

	if (LIKELY(TTime)) {
		const UFlags* const Flags = UFlags::Instance(this);
		const float Time = Flags ? Flags->Get(LDConsts::Flags::Stats::Global::Time) : 0;
		const FTimespan Ts(0,0,Time);
		static const FText Fmt = NSLOCTEXT("LSettingsUI", "Time", "Time: {0}");
		const FText TimeText = FText::FormatOrdered(Fmt, FText::AsTimespan(Ts));
		TTime->SetText(TimeText);
	}
}

void ULSettingsUI::NativeOnInitialized() {
	Super::NativeOnInitialized();

	if (LIKELY(CBScale)) {
		CBScale->ClearOptions();
		CBScale->AddOption("UI Scale");
		const float ScaleCur = UJUtilsMisc::GetUIScale();
		int32 SelectedI = 0;
		for (uint8 i = 1; i<LSetUI::ScalesLen; ++i) {//skip initial
			const float S = LSetUI::Scales[i];
			CBScale->AddOption(FString::Printf(TEXT("%.2fx Scale"), S));
			// selects the last one that's smaller than the current one.
			// will pick the largest one that is not larger than the value.
			if (S<=ScaleCur) SelectedI = i;
		}
		CBScale->SetSelectedIndex(SelectedI);
		CBScale->OnSelectionChanged.AddUniqueDynamic(this, &ULSettingsUI::ScaleUpd);
	}

	const bool Dbg = ULSettings::GetFeatS(this, EFeat::DBG_BASE);
	if (LIKELY(BtnOptDbg)) {
		BtnOptDbg->SetVisibility(Dbg ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (UNLIKELY(Dbg)) BtnOptDbg->OnClicked.AddUniqueDynamic(this, &ULSettingsUI::ShowDbg);
	}
}

void ULSettingsUI::NativeDestruct() {
	if (LIKELY(CBScale)) CBScale->OnSelectionChanged.RemoveAll(this);

	Super::NativeDestruct();
}

void ULSettingsUI::ScaleUpd(const FString SelectedItem, const ESelectInfo::Type SelectionType) {
	if (UNLIKELY(!CBScale)) return;

	const int32 I = CBScale->GetSelectedIndex();
	if (UNLIKELY(I<1 || I>=LSetUI::ScalesLen)) return; // actually ignore 0
	UJUtilsMisc::SetUIScale(LSetUI::Scales[I]);
}

void ULSettingsUI::ShowDbg() {
	if (LIKELY(SWOptions)) SWOptions->SetActiveWidgetIndex(5);
}

void ULSettingsUI::SetPause() {
	UGameplayStatics::SetGamePaused(this, true);
}

void ULSettingsUI::TimeUpd(UObject* const Obj, const FName Name, const float Alpha) {
	// because this animation is affected by the time dilation. the time is an exponential curve.
	// in other words, it's going to slow down logarithmically. if the min dilation is too small, it could take forever.
	// since the timer and the anim both use the same duration, this animation won't really reach the end. so we cancel it on hide.
	UGameplayStatics::SetGlobalTimeDilation(this, FMath::Lerp(1, 0.2, Alpha));
}
