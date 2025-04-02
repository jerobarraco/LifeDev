// Copyright Jerónimo Barraco-Mármol

#include "LSwitchI00.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Interact/Animator/CAnimatorMix.h"

ALSwitchI00::ALSwitchI00():Super() {
	IsOneShot = true;
	// only used for the ch0, kinda unnecessary, but...
	UseActiveOnce = true;
	StateNum = 2;
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/Interact/Interact_C.Interact_C"));
	Anim->Curve = CCurve.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CPart(TEXT("/Game/LifeDev/Game/Inters/Bath/HandBasin/WaterTap"));
	Particles = { nullptr, CPart.Object}; // TODO test
	Emitter->SetRelativeLocation(FVector(-20,2.5,-45));
}
// the other switch is bound on outliner