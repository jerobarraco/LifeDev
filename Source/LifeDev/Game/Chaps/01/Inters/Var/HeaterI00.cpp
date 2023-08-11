// Copyright Jerónimo Barraco-Mármol

#include "HeaterI00.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Math/CodeCurve.h"

AHeaterI00::AHeaterI00():Super() {
	AnimEnabled = true;
	Interact->SetEnabled(true);
	
	Anim->Duration = .75f;
	TriggerDlg = "HT00_T";
	LockedDlg = "HT00_L";
	Texts = {
		FText::FromString(TEXT("Turn on. It's cold")),
		FText::FromString(TEXT("Maybe not..."))
	};


	// test
	Anim->Curve = nullptr;
	UCodeCurveLib* Lib = NewObject<UCodeCurveLib>();
	Anim->CodeCurve.BindDynamic(Lib, &UCodeCurveLib::UCodeCurveLib::SinFull);
}

void AHeaterI00::Trigger_Implementation() {
	Super::Trigger_Implementation();
	Locked = true;
}

void AHeaterI00::TriggerLocked_Implementation() {
	Super::TriggerLocked_Implementation();
}
