// Copyright Jerónimo Barraco-Mármol

#include "Clout.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorMix.h"

AClout::AClout():Super() {
	Super::SetMobility(EComponentMobility::Static);
	Anim->TRoot = nullptr;
	Anim->Mat = nullptr;
	Texts = {FText::FromString("PickUp")};
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Clouts.Clouts"));
	SFX_Trigger = CSnd.Object;
	RewardFlash = .1;
	// a bit lazy, ideally should be set to true on each child instance.
	// Assumes all children will fade. will create a new material.
	UseFade = true;
	Mesh->SetCastAllShadows(true);
}
