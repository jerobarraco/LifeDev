// Copyright Jerónimo Barraco-Mármol

#include "HeaterI00.h"

#include "Diags/Diags.h"
#include "Interact/Animator/CAnimatorMix.h"

#include "LifeDev/Core/Consts/ConstItems.h"

AHeaterI00::AHeaterI00():Super() {
	RewardFlash = .1f;
	// don't disappear
	UseRewardDestroy = false;
	UseFade = false;
	UseRewardDestroy = false;
	UseAnim = true;
	UseAutoActivate = true;

	Texts = {
		NSLOCTEXT("HeaterI00", "State0", "Turn on, it's cold"),
		NSLOCTEXT("HeaterI00", "State1", "Maybe not...")
	};

	// animation
	Anim->IsAdditive = true;
	Anim->SetComponentTickInterval(1/60.f);
	Anim->TEnd.SetRotation(FRotator(-10,0,0).Quaternion());
	Anim->Duration = .75f;
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/JUtils/Curves/BounceOut_C.BounceOut_C"));
	if (LIKELY(CCurve.Succeeded())) Anim->Curve = CCurve.Object;

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Heater00/dry_hard_metal_grind_09-01.dry_hard_metal_grind_09-01"));
	SFX_Trigger = CSnd.Object;
}

void AHeaterI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	IsLocked = true; // lock AFTER triggering
}

EItemUseResult AHeaterI00::TryUseItem_Implementation(const FName& Name) {
	if (Name == LDConsts::Items::Card0 && LIKELY(IsValid(Diags))) {
		Diags->AddId("HT00_IC0");
		return EItemUseResult::BAD_HANDLED;
	}

	return Super::TryUseItem_Implementation(Name);
}
