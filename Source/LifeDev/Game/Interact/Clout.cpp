// Copyright Jerónimo Barraco-Mármol

#include "Clout.h"

#include "Interact/Animator/CAnimatorMix.h"

AClout::AClout():Super() {
	AnimEnabled = false;
	Anim->TRoot = nullptr;
	Anim->Mat = nullptr;
	Texts = {FText::FromString("PickUp")};
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Clouts/Clouts.Clouts"));
	SFX_Trigger = CSnd.Object;
	IRoot->SetMobility(EComponentMobility::Static);
	Mesh->SetMobility(EComponentMobility::Static);
	RootComponent->SetMobility(EComponentMobility::Static);
	TriggerFlashInc = .1;
}
