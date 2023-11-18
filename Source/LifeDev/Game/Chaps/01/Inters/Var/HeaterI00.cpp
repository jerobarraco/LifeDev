// Copyright Jerónimo Barraco-Mármol

#include "HeaterI00.h"

#include "Diags/Diags.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

AHeaterI00::AHeaterI00():Super() {
	RewardFlash = .1f;
	// don't disappear
	UseRewardFade = false;

	TriggerDlg = "HT00_T";
	LockedDlg = "HT00_L";
	Texts = {
		FText::FromString(TEXT("Turn on. It's cold")),
		FText::FromString(TEXT("Maybe not..."))
	};

	// animation
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/JUtils/Curves/BounceOut_C.BounceOut_C"));
	if (CCurve.Succeeded())
		Anim->Curve = CCurve.Object;
	Anim->SetComponentTickInterval(1/60.f);
	Anim->TEnd.SetRotation(FRotator(-10,0,0).Quaternion());
	Anim->Duration = .75f;
	Anim->IsAdditive = true;
	AnimEnabled = true;
	Interact->SetEnabled(true);

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Heater00/dry_hard_metal_grind_09-01.dry_hard_metal_grind_09-01"));
	SFX_Trigger = CSnd.Object;
}

void AHeaterI00::Trigger_Implementation() {
	Super::Trigger_Implementation();
	Locked = true; // lock AFTER triggering
}

EItemUseResult AHeaterI00::TryUseItem_Implementation(const FName& Name) {
	if (Name == LDConsts::Items::Card0 && IsValid(Dialogs)) {
		Dialogs->AddId("HT00_IC0");
		return EItemUseResult::BAD_HANDLED;
	}

	return Super::TryUseItem_Implementation(Name);
}
