// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetGameUI.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "LifeDev/Game/Dialogs/LDiagMan.h"

void ULSetGameUI::Apply_Implementation() {
	Super::Apply_Implementation();
	if (LIKELY(SLDiagAutoTime)) {
		ALDiagMan::CFGSetAutoTime(this, SLDiagAutoTime->GetValue());
	}
}

void ULSetGameUI::Load_Implementation() {
	Super::Load_Implementation();
	const float AutoTime = ALDiagMan::CFGGetAutoTime();
	if (LIKELY(SLDiagAutoTime))
		SLDiagAutoTime->SetValue(AutoTime);
	DiagAutoTimeUpd(AutoTime); // not called automatically
}

void ULSetGameUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (LIKELY(SLDiagAutoTime)) {
		SLDiagAutoTime->SetMaxValue(1*60);
		SLDiagAutoTime->SetMinValue(.2);
		SLDiagAutoTime->OnValueChanged.AddUniqueDynamic(this, &ULSetGameUI::DiagAutoTimeUpd);
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
