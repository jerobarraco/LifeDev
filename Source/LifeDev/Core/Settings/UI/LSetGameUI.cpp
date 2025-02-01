// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetGameUI.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Interact/Interact.h"
#include "LifeDev/Game/Char/LChar.h"

#include "LifeDev/Game/Dialogs/LDiagMan.h"
#include "LifeDev/Game/Flashback/Flashback.h"

void ULSetGameUI::Apply_Implementation() {
	Super::Apply_Implementation();
	if (LIKELY(SLDiagAutoTime)) {
		ALDiagMan* const Man = ALDiagMan::InstanceL(this);
		if (LIKELY(Man)) {
			Man->AutoTime = FMath::Max(.05, SLDiagAutoTime->GetValue());
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
			Inter->HintTime = FMath::Max(2, SLInterHint->GetValue());
			Inter->SaveConfig();
		}
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

	const ALChar* const Char = ALChar::Instance(this);
	const float InterDrag = Char ? Char->InteractDrag : .5;
	if (LIKELY(SLInterDrag)) SLInterDrag->SetValue(InterDrag);
	InterDragUpd(InterDrag);
	
	const AInteract* const Inter = GetMutableDefault<AInteract>();
	const float InterHint = Inter ? Inter->HintTime : 1.5;
	if (LIKELY(SLInterHint)) SLInterHint->SetValue(InterHint);
	InterDragUpd(InterHint);
}

void ULSetGameUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (LIKELY(SLDiagAutoTime)) {
		SLDiagAutoTime->SetMaxValue(1*60);
		SLDiagAutoTime->SetMinValue(.05);
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
	static FText Fmt = NSLOCTEXT("ULSetGameUI", "TInterDrag", "{0} secs");
	if (LIKELY(TInterHint)) {
		static FNumberFormattingOptions NFOption;
		NFOption.MaximumFractionalDigits = 3;
		NFOption.MinimumFractionalDigits = 3;
		const FText Num = FText::AsNumber(Value, &NFOption);
		TInterHint->SetText(FText::Format(Fmt, Num));
	}
}
