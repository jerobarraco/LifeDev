// Copyright Jerónimo Barraco-Mármol

#include "HeaterI00.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

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

	/*
	 Kinda works
	UCurveFloat* CurveFloat = NewObject<UCurveFloat>();
	FRichCurve& F = CurveFloat->FloatCurve;

	// 0, 0,  0.42, 0.0,  0.58, 1,  1, 1
	F.UpdateOrAddKey(0, 0);
	F.UpdateOrAddKey(0, .42);
	F.UpdateOrAddKey(1, .58);
	F.UpdateOrAddKey(1, 1); 
	F.AutoSetTangents();
	Anim->Curve = CurveFloat;

	*/
}

void AHeaterI00::Trigger_Implementation() {
	Super::Trigger_Implementation();
	Locked = true;
}
