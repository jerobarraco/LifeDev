// Copyright Jerónimo Barraco-Mármol

#include "SpongeI00.h"

#include "Interact/Animator/CAnimatorMix.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ASpongeI00::ASpongeI00():Super() {
	TriggerDlg = "Sponge00_T";
	RewardItem = LDConsts::Items::Plate01;
	RewardStep = true;
	Locked = false;
	UseRewardDestroy = false;
	IsOneShot = true;
	UseAnim = true;
	SetEnabled(false);
	// TODO if i have time, on trigger, fade out, then fade in and do reward
	// might be easier to do with an extra step

	StateNum = 2;
	Trans = {
		FTransform(
			FRotator(-4.923850,0.870385, -10.037423),
			FVector(0,-5,10)
		),
		FTransform::Identity
	};
	IRoot->SetRelativeTransform(Trans[1]);
	Anim->IsAdditive = false;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Kitchen/washing_hands_inside_a_bathroom_mostly_empty_soap_pump_sink_on_then_off_edit"));
	SFX_Trigger = CSnd.Object;
}

void ASpongeI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	for (ALInteract* const I: Plates) {
		if (!IsValid(I)) continue;
		I->Fade(false);
	}
	Plates.Empty();
}

void ASpongeI00::BeginPlay() {
	Super::BeginPlay();
	for (ALInteract* const I: Plates) {
		if (!IsValid(I)) continue;
		I->Fade(true);
		I->SetEnabled(false); // force them disabled.
	}
}
