// Copyright Jerónimo Barraco-Mármol

#include "HeaterI00.h"

#include "Diags/Diags.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

AHeaterI00::AHeaterI00():Super() {
	RewardFlash = .1f;
	// don't disappear
	UseRewardDestroy = false;

	TriggerDlg = "HT00_T";
	LockedDlg = "HT00_L";
	Texts = {
		FText::FromString(TEXT("Turn on, it's cold")),
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
	UseAnim = true;
	Interact->SetActive(true);

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Heater00/dry_hard_metal_grind_09-01.dry_hard_metal_grind_09-01"));
	SFX_Trigger = CSnd.Object;
}

void AHeaterI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	Locked = true; // lock AFTER triggering
}

EItemUseResult AHeaterI00::TryUseItem_Implementation(const FName& Name) {
	if (Name == LDConsts::Items::Card0 && IsValid(Diags)) {
		Diags->AddId("HT00_IC0");
		return EItemUseResult::BAD_HANDLED;
	}

	return Super::TryUseItem_Implementation(Name);
}
