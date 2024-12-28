// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetGameUI.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"

#include "LifeDev/Game/Dialogs/LDiagMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"

void ULSetGameUI::Apply_Implementation() {
	Super::Apply_Implementation();
	if (LIKELY(SLDiagAutoTime)) {
		ALDiagMan* const Man = ALDiagMan::InstanceL(this);
		if (LIKELY(Man)) Man->SetAutoTime(SLDiagAutoTime->GetValue());
	}
	if (LIKELY(SLFBTime)) {
		UFlashback* const Flash = UFlashback::Instance(this);
		Flash->AnimTime = FMath::Max(.1, SLFBTime->GetValue());
		Flash->SaveConfig();
	}
}

void ULSetGameUI::Load_Implementation() {
	Super::Load_Implementation();
	float AutoTime = 0;
	const ALDiagMan* const Man = ALDiagMan::InstanceL(this);
	if (LIKELY(Man)) AutoTime = Man->GetAutoTime();

	if (LIKELY(SLDiagAutoTime))
		SLDiagAutoTime->SetValue(AutoTime);
	DiagAutoTimeUpd(AutoTime); // not called automatically
	
	const UFlashback* const Flash = UFlashback::Instance(this);
	const float FBAnimTime = Flash ? Flash->AnimTime : 20;
	if (LIKELY(SLFBTime)) SLFBTime->SetValue(FBAnimTime);
	FBTimeUpd(FBAnimTime);
}

void ULSetGameUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (LIKELY(SLDiagAutoTime)) {
		SLDiagAutoTime->SetMaxValue(1*60);
		SLDiagAutoTime->SetMinValue(.1);
		SLDiagAutoTime->OnValueChanged.AddUniqueDynamic(this, &ULSetGameUI::DiagAutoTimeUpd);
	}
	if (LIKELY(SLFBTime)) {
		SLFBTime->SetMaxValue(60);
		SLFBTime->SetMinValue(.1);
		SLFBTime->OnValueChanged.AddUniqueDynamic(this, &ULSetGameUI::FBTimeUpd);
	}
}

void ULSetGameUI::DiagAutoTimeUpd(const float Value) {
	static FText Fmt = NSLOCTEXT("ULSetGameUI", "TDiagAutoTime", "{0} secs.");
	if (LIKELY(TDiagAutoTime)) {
		static FNumberFormattingOptions NFOption;
		NFOption.MaximumFractionalDigits = 4;
		NFOption.MinimumFractionalDigits = 4;
		const FText Num = FText::AsNumber(Value, &NFOption);
		TDiagAutoTime->SetText(FText::Format(Fmt, Num));
	}
}

void ULSetGameUI::FBTimeUpd(const float Value) {
	static FText Fmt = NSLOCTEXT("ULSetGameUI", "TFBTime", "{0} secs.");
	if (LIKELY(TFBTime)) {
		static FNumberFormattingOptions NFOption;
		NFOption.MaximumFractionalDigits = 4;
		NFOption.MinimumFractionalDigits = 4;
		const FText Num = FText::AsNumber(Value, &NFOption);
		TFBTime->SetText(FText::Format(Fmt, Num));
	}
}
