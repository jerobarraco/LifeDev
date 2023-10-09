// Copyright Jerónimo Barraco-Mármol

#include "LLight.h"

#include "Interact/Animator/CAnimatorMix.h"
// better to do light00 first then extract this one
ALLight::ALLight():Super() {
	Super::SetMobility(EComponentMobility::Static);
	Anim->TRoot = nullptr;
	Anim->Mat = nullptr;
	Super::SetEnabled(false);
	// Texts = {FText::FromString("PickUp")};
	// static ConstructorHelpers::FObjectFinder<USoundBase>
		// CSnd (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/LLights/LLights.LLights"));
	// SFX_Trigger = CSnd.Object;
	// TriggerFlashInc = .1;
}
