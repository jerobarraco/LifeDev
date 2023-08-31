// Copyright Jerónimo Barraco-Mármol

#include "Clout.h"

#include "Interact/Animator/CAnimatorMix.h"

AClout::AClout():Super() {
	Super::SetMobility(EComponentMobility::Static);
	Anim->TRoot = nullptr;
	Anim->Mat = nullptr;
	Texts = {FText::FromString("PickUp")};
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Clouts/Clouts.Clouts"));
	SFX_Trigger = CSnd.Object;
	TriggerFlashInc = .1;
}
