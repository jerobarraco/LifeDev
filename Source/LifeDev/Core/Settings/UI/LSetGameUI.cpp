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
	if (LIKELY(SLDiagAutoTime)) {
		SLDiagAutoTime->SetMaxValue(3*60);
		SLDiagAutoTime->SetMinValue(.2);
		SLDiagAutoTime->SetValue(AutoTime);
	}

	static FText Fmt = NSLOCTEXT("ULSetGameUI", "TDiagAutoTime", "{0} seconds");
	if (LIKELY(TDiagAutoTime))
		TDiagAutoTime->SetText(
			FText::Format(Fmt, FText::FromString(FString::SanitizeFloat(AutoTime))));
}
