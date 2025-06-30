// Copyright Jerónimo Barraco-Mármol

#include "Towel.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorMix.h"

ATowel::ATowel():Super() {
	StateNum = 1;
	RewardFlash = 0;
	UseFade = true;
	Texts = { NSLOCTEXT("Towel", "State0", "Towel")};
	UseAutoActivate = false;
	UseAnim = false;
	Anim->TRoot = nullptr;
	Anim->Mat = nullptr;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Towel00_P"));

	// TODO
	// SFXs = {CSnd.Object};
	
	Mesh->SetCastAllShadows(true);
	Super::SetMobility(EComponentMobility::Static);
}
