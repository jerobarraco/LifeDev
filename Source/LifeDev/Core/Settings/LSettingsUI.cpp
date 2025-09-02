// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSettingsUI.h"


#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

#include "Interact/Animator/Anim.h"
#include "Inventory/Flags.h"
#include "JUtils/Misc/JUtilsMisc.h"
#include "JUtils/UI/GroupBox.h"
#include "JButton.h"

#include "LifeDev/Core/Sentry.h"
#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "UI/LSetDbgUI.h"
#include "UI/LMsgBox.h"
#include "LSettings.h"

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
	if (LIKELY(Man)) Man->FadeMusic(true);

	UAnim* const Anim = UAnim::Instance(this);
	if (LIKELY(Anim)) {
		// i could ignore the dilation, but letting this get affected by it makes it look exponential, which i like.
		const FAParams P {.Duration = PauseTime};
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
	if (LIKELY(Man)) Man->FadeMusic(false);

	UAnim* const Anim = UAnim::Instance(this);
	if (LIKELY(Anim)) {
		// i could ignore the dilation, but letting this get affected by it makes it look exponential, which i like.
		const FAParams P {.Duration = PauseTime};
		Anim->TimeFade(this, P, 1);
	}

	Super::Hide_Implementation();

	UGameplayStatics::SetGamePaused(this, false);
	// SetVisibility(ESlateVisibility::Collapsed);
}

void ULSettingsUI::Apply_Implementation() {
	if (LIKELY(Settings_Dbg)) Settings_Dbg->Apply();
}

void ULSettingsUI::Load_Implementation() {
	if (LIKELY(Settings_Dbg)) Settings_Dbg->Load();

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

	if (LIKELY(BtnComment)) {
		BtnComment->SetUp(NSLOCTEXT("Settings", "BtnComment", "Send Feedback"));
		BtnComment->OnClick.AddUniqueDynamic(this, &ULSettingsUI::SendComment);
	}
}

void ULSettingsUI::NativeDestruct() {
	if (LIKELY(CBScale)) CBScale->OnSelectionChanged.RemoveAll(this);
	if (LIKELY(BtnOptDbg))
		BtnOptDbg->OnClicked.RemoveAll(this);

	if (LIKELY(BtnComment))
		BtnComment->OnClick.RemoveAll(this);

	Super::NativeDestruct();
}

void ULSettingsUI::ScaleUpd(const FString SelectedItem, const ESelectInfo::Type SelectionType) {
	if (UNLIKELY(!CBScale)) return;

	const int32 I = CBScale->GetSelectedIndex();
	if (UNLIKELY((I<1) | (I>=LSetUI::ScalesLen))) return; // actually ignore 0, since that's just "scale"
	UJUtilsMisc::SetUIScale(LSetUI::Scales[I]);
}

void ULSettingsUI::ShowDbg() {
	if (LIKELY(SWOptions)) SWOptions->SetActiveWidgetIndex(5);
}

void ULSettingsUI::SetPause() {
	UGameplayStatics::SetGamePaused(this, true);
}

void ULSettingsUI::SendComment(const int32 Id) {
	const USentry* const Sentry = USentry::Instance(this);
	if (UNLIKELY(!Sentry | !TComment )) return;

	Sentry->AddComment(TComment->GetText().ToString());
	if (LIKELY(MsgBox)) {
		static const FText& Sent = NSLOCTEXT("SettingsUI", "SentComment", "Sent! Thanks a lot!");
		static const FText& Nice = NSLOCTEXT("SettingsUI", "SentCommentBtn", "Nice!");
		MsgBox->SetUp(Sent, {Nice});
		MsgBox->Show();
		TComment->SetText(FText::FromString(""));
	}
}
