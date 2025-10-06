// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetGameUI.h"

#include "JButton.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "HAL/FileManagerGeneric.h"

#include "Interact/Interact.h"
#include "JUtils/Misc/JUtilsSys.h"

#include "LFeatsGroup.h"
#include "LifeDev/Core/Settings/LFeatsMan.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Dialogs/LDiagMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"

void ULSetGameUI::Apply_Implementation() {
	Super::Apply_Implementation();
	if (LIKELY(SLDiagAutoTime)) {
		ALDiagMan* const Man = ALDiagMan::InstanceL(this);
		if (LIKELY(Man)) {
			Man->AutoTime = SLDiagAutoTime->GetValue();
			Man->SaveConfig();
		}
	}

	if (LIKELY(SLFBTime)) {
		UFlashback* const Flash = UFlashback::Instance(this);
		if (LIKELY(Flash)) {
			Flash->AnimTime = FMath::Max(.1, SLFBTime->GetValue());
			Flash->SaveConfig();
		}
	}

	if (LIKELY(SLInterDrag)) {
		ALChar* const Char = ALChar::Instance(this);
		if (LIKELY(Char)) {
			Char->InteractDrag = FMath::Max(.1, SLInterDrag->GetValue());
			Char->SaveConfig();
		}
	}

	if (LIKELY(SLInterHint)) {
		AInteract* const Inter = GetMutableDefault<AInteract>();
		if (LIKELY(Inter)) {
			Inter->HintTime = FMath::Max(.001, SLInterHint->GetValue());
			Inter->SaveConfig();
		}
	}

	// use the in-level if possible
	ALFeatsMan* Man = ALFeatsMan::Instance(this);
	// otherwise the default
	if (UNLIKELY(!Man)) Man = GetMutableDefault<ALFeatsMan>();

	if (LIKELY(bool(SLBlurSize) & bool(Man))) {
		Man->MotionBlurAmount = SLBlurSize->GetValue();
		Man->SaveConfig();
		Man->BlurReset(); // i can do this because i check nullptrs
	}
	if (LIKELY(bool(SLFringe) & bool(Man))) {
		Man->FringeIntensity = SLFringe->GetValue();
		Man->SaveConfig();
		Man->FringeReset(); // i can do this because i check nullptrs
	}

	if (LIKELY(FeatsGroup)) FeatsGroup->Apply();

	if (LIKELY(Langs) && Langs->IsVisible()) { // is visible because it's feature flagged
		const FString& New = Langs->GetSelectedOption();
		if (New != UJUtilsSys::GetCulture()) 
			UJUtilsSys::SetCulture(New);
	}
}

void ULSetGameUI::Load_Implementation() {
	Super::Load_Implementation();

	float AutoTime = 0;
	const ALDiagMan* const Man = ALDiagMan::InstanceL(this); // this probably will only be changeable during gameplay and not intro
	if (LIKELY(Man)) AutoTime = Man->AutoTime;

	if (LIKELY(SLDiagAutoTime))
		SLDiagAutoTime->SetValue(AutoTime);
	DiagAutoTimeUpd(AutoTime); // not called automatically
	
	const UFlashback* const Flash = UFlashback::Instance(this);
	const float FBAnimTime = Flash ? Flash->AnimTime : 20;
	if (LIKELY(SLFBTime)) SLFBTime->SetValue(FBAnimTime);
	FBTimeUpd(FBAnimTime);

	const ALChar* const Char = ALChar::Instance(this);
	const float InterDrag = Char ? Char->InteractDrag : .5;
	if (LIKELY(SLInterDrag)) SLInterDrag->SetValue(InterDrag);
	InterDragUpd(InterDrag);
	
	const AInteract* const Inter = GetMutableDefault<AInteract>();
	const float InterHint = Inter ? Inter->HintTime : 1.5;
	if (LIKELY(SLInterHint)) SLInterHint->SetValue(InterHint);
	InterDragUpd(InterHint);

	// use the in-level if possible
	ALFeatsMan* FMan = ALFeatsMan::Instance(this);
	if (UNLIKELY(!FMan)) FMan = GetMutableDefault<ALFeatsMan>();
	const float BlurSize = LIKELY(FMan) ? FMan->MotionBlurAmount : 1;
	if (LIKELY(SLBlurSize)) SLBlurSize->SetValue(BlurSize);
	BlurSizeUpd(BlurSize);

	const float FringeSize = LIKELY(FMan) ? FMan->FringeIntensity : 1;
	if (LIKELY(SLFringe)) SLFringe->SetValue(FringeSize);
	FringeUpd(FringeSize);

	if (LIKELY(FeatsGroup)) FeatsGroup->Load();
}

void ULSetGameUI::ClearConfig() {
	if (UNLIKELY(UJUtilsSys::IsEditor())) {
		UE_LOG(LogTemp, Log, TEXT("%hs What? i just saved you. i can't let you do that dave."), __func__);
		return;
	}

	const FString& Dir = FPaths::Combine(FPaths::ProjectUserDir(), "Saved", "Config");
	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();
	UE_LOG(LogTemp, Log, TEXT("%hs Nuking %s"), __func__, *Dir);
	if (File.DirectoryExists(*Dir))
		FFileManagerGeneric::Get().DeleteDirectory(*Dir, false, true);
}

void ULSetGameUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (LIKELY(SLDiagAutoTime)) {
		SLDiagAutoTime->SetMaxValue(1*60);
		SLDiagAutoTime->SetMinValue(.025);
		SLDiagAutoTime->OnValueChanged.AddUniqueDynamic(this, &ULSetGameUI::DiagAutoTimeUpd);
	}

	if (LIKELY(SLFBTime)) {
		SLFBTime->SetMaxValue(60);
		SLFBTime->SetMinValue(.1);
		SLFBTime->OnValueChanged.AddUniqueDynamic(this, &ULSetGameUI::FBTimeUpd);
	}

	if (LIKELY(SLInterDrag)) {
		SLInterDrag->SetMaxValue(1.);
		SLInterDrag->SetMinValue(.1);
		SLInterDrag->OnValueChanged.AddUniqueDynamic(this, &ULSetGameUI::InterDragUpd);
	}

	if (LIKELY(SLInterHint)) {
		SLInterHint->SetMaxValue(10.);
		SLInterHint->SetMinValue(2); // the range animation time
		SLInterHint->OnValueChanged.AddUniqueDynamic(this, &ULSetGameUI::InterHintUpd);
	}

	if (LIKELY(SLBlurSize)) {
		SLBlurSize->SetMaxValue(5.);
		SLBlurSize->SetMinValue(0);
		SLBlurSize->OnValueChanged.AddUniqueDynamic(this, &ULSetGameUI::BlurSizeUpd);
	}

	if (LIKELY(SLFringe)) {
		SLFringe->SetMaxValue(5);
		SLFringe->SetMinValue(0);
		SLFringe->OnValueChanged.AddUniqueDynamic(this, &ULSetGameUI::FringeUpd);
	}

	if (LIKELY(FeatsGroup)) FeatsGroup->SetUp({
		{EFeat::E_GHOSTPOOL, NSLOCTEXT("SetGame", "Feat", "Ghosts")},
		{EFeat::D_AUTO, NSLOCTEXT("SetGame", "Feat", "Diag. Auto")},
		{EFeat::D_TEXT, NSLOCTEXT("SetGame", "Feat", "Diag. Text")},
		{EFeat::G_CARD0, NSLOCTEXT("SetGame", "Feat", "C. Intention")},
		{EFeat::G_CARD1, NSLOCTEXT("SetGame", "Feat", "C. 1")},
		{EFeat::G_CARD2, NSLOCTEXT("SetGame", "Feat", "C. 2")},
		{EFeat::G_CARD3, NSLOCTEXT("SetGame", "Feat", "C. 3")},
		{EFeat::G_TEACH, NSLOCTEXT("SetGame", "Feat", "Tutorial")},
		{EFeat::G_FB_ANIM, NSLOCTEXT("SetGame", "Feat", "AnimFBFPS")},
		{EFeat::G_SHOW_POINT, NSLOCTEXT("SetGame", "Feat", "Pointer")}, // this is wip since it gets rewritten in bp
		{EFeat::G_STATUS, NSLOCTEXT("SetGame", "Feat", "Status")},
		{EFeat::U_TICK_CON, NSLOCTEXT("SetGame", "Feat", "Tick Concurr.")},
		{EFeat::U_TICK_BATCH, NSLOCTEXT("SetGame", "Feat", "Tick Batched")},
		{EFeat::G_SAVE, NSLOCTEXT("SetGame", "Feat", "Save (Care!)")},
	});

	if (LIKELY(Langs)) {
		const bool IsDebug = ULSettings::GetFeatS(this, EFeat::DBG_BASE);
		Langs->SetVisibility(IsDebug ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		if (IsDebug) {
			// TArray<FString> Cultures;
			// UJUtilsSys::GetAllCultures(Cultures); // this one list all POSSIBLE not all supported :(
			TArray<FString> Cultures = {"en", "es"};
			for (const FString& C: Cultures)
				Langs->AddOption(C);

			const FString& Cur = UJUtilsSys::GetCulture();
			Langs->SetSelectedOption(Cur);
		}
	}

	if (LIKELY(BtnDefaults))
		BtnDefaults->OnClick.AddUniqueDynamic(this, &ULSetGameUI::SetDefaults);
}

void ULSetGameUI::NativeDestruct() {
	// todo the rest of the things?
	if (LIKELY(BtnDefaults))
		BtnDefaults->OnClick.RemoveAll(this);
	Super::NativeDestruct();
}

void ULSetGameUI::SetDefaults(const int32 Id) {
	static const float AutoTime = ALDiagMan::DefAutoTime;
	if (LIKELY(SLDiagAutoTime)) SLDiagAutoTime->SetValue(AutoTime);
	DiagAutoTimeUpd(AutoTime); // not called automatically

	if (LIKELY(SLFringe)) SLFringe->SetValue(1);
	
	FeatsGroup->SetDefaults();
}

void ULSetGameUI::DiagAutoTimeUpd(const float Value) {
	static FText Fmt = NSLOCTEXT("ULSetGameUI", "TDiagAutoTime", "{0} secs.");
	if (LIKELY(TDiagAutoTime)) {
		static FNumberFormattingOptions NFOption;
		NFOption.MaximumFractionalDigits = 3;
		NFOption.MinimumFractionalDigits = 3;
		const FText Num = FText::AsNumber(Value, &NFOption);
		TDiagAutoTime->SetText(FText::Format(Fmt, Num));
	}
}

void ULSetGameUI::FBTimeUpd(const float Value) {
	static FText Fmt = NSLOCTEXT("ULSetGameUI", "TFBTime", "{0} secs.");
	if (LIKELY(TFBTime)) {
		static FNumberFormattingOptions NFOption;
		NFOption.MaximumFractionalDigits = 3;
		NFOption.MinimumFractionalDigits = 3;
		const FText Num = FText::AsNumber(Value, &NFOption);
		TFBTime->SetText(FText::Format(Fmt, Num));
	}
}

void ULSetGameUI::InterDragUpd(const float Value) {
	static FText Fmt = NSLOCTEXT("ULSetGameUI", "TInterDrag", "{0} multiplier");
	if (LIKELY(TInterDrag)) {
		static FNumberFormattingOptions NFOption;
		NFOption.MaximumFractionalDigits = 3;
		NFOption.MinimumFractionalDigits = 3;
		const FText Num = FText::AsNumber(Value, &NFOption);
		TInterDrag->SetText(FText::Format(Fmt, Num));
	}
}

void ULSetGameUI::InterHintUpd(const float Value) {
	static FText Fmt = NSLOCTEXT("ULSetGameUI", "TInterHint", "{0} secs");
	if (LIKELY(TInterHint)) {
		static FNumberFormattingOptions NFOption;
		NFOption.MaximumFractionalDigits = 3;
		NFOption.MinimumFractionalDigits = 3;
		const FText Num = FText::AsNumber(Value, &NFOption);
		TInterHint->SetText(FText::Format(Fmt, Num));
	}
}

void ULSetGameUI::BlurSizeUpd(const float Value) {
	if (UNLIKELY(!TBlurSize)) return;

	static FText Fmt = NSLOCTEXT("ULSetGameUI", "TBlurSize", "{0}");
	static FNumberFormattingOptions NFOption;
	NFOption.MaximumFractionalDigits = 3;
	NFOption.MinimumFractionalDigits = 3;
	const FText Num = FText::AsNumber(Value, &NFOption);
	TBlurSize->SetText(FText::Format(Fmt, Num));
}

void ULSetGameUI::FringeUpd(const float Value) {
	if (UNLIKELY(!TFringe)) return;

	static FText Fmt = NSLOCTEXT("ULSetGameUI", "TFringe", "{0}");
	static FNumberFormattingOptions NFOption;
	NFOption.MaximumFractionalDigits = 3;
	NFOption.MinimumFractionalDigits = 3;
	const FText Num = FText::AsNumber(Value, &NFOption);
	TFringe->SetText(FText::Format(Fmt, Num));
}

