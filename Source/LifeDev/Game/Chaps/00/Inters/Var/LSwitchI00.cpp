// Copyright Jerónimo Barraco-Mármol

#include "LSwitchI00.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Interact/Animator/CAnimatorMix.h"

ALSwitchI00::ALSwitchI00():Super() {
	IsOneShot = true;
	// only used for the ch0, kinda unnecessary, but...
	UseActiveOnce = true;
	UseAutoActivate = true;
	UseRewardDestroy = false;
	StateNum = 2;
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/Interact/Interact_C.Interact_C"));
	Anim->Curve = CCurve.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CPart(TEXT("/Game/LifeDev/Game/Inters/Bath/HandBasin/WaterTap"));
	Particles = { nullptr, CPart.Object};
	Emitter->SetRelativeLocation(FVector(20,-5,-38));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Bath/Toilet02/OLD_STUFF_Toilet_Tank_Pull_Chain_02"));
	SFXs = {CSnd.Object};
}
// the other switch is bound on outliner