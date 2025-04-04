// Copyright Jerónimo Barraco-Mármol

#include "Clout.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorMix.h"

AClout::AClout():Super() {
	RewardFlash = .1;
	// a bit lazy, ideally should be set to true on each child instance.
	// Assumes all children will fade. will create a new material.
	UseFade = true;
	Texts = { NSLOCTEXT("Clout", "State0", "Pick up")};
	// kinda safe, since if i want a clout that is not active i could use a static mesh.
	// but that is a lie. i might need something that is not active BY DEFAULT, but active later.
	// but that's: UNLIKELY(that)
	UseAutoActivate = true;

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Clouts.Clouts"));
	SFX_Trigger = CSnd.Object;
	Mesh->SetCastAllShadows(true);

	UseAnim = false;
	Anim->TRoot = nullptr;
	Anim->Mat = nullptr;
	Super::SetMobility(EComponentMobility::Static);
}
