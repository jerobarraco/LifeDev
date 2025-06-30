// Copyright Jerónimo Barraco-Mármol

#include "Towel.h"

#include "CQuickMesh.h"
#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

ATowel::ATowel():Super() {
	StateNum = 1;
	RewardFlash = 0;
	UseFade = true;
	Texts = { NSLOCTEXT("Towel", "State0", "Towel")};
	UseAutoActivate = false;
	UseAnim = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Utensils/Cup00_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	// TODO
	Mesh->SetRelativeLocation(FVector(-5.500000,5.25,0));
	Mesh->SetCastAllShadows(true);

	// TODO
	Interact->SetRelativeLocation(FVector(10,-5,12.5));
	Interact->SetBoxExtent(FVector(15,7.5,15));
	SFX->SetRelativeLocation(FVector(10,-5,12.5));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Towel00_P"));

	// TODO
	// SFXs = {CSnd.Object};
	
	Mesh->SetCastAllShadows(true);
	Super::SetMobility(EComponentMobility::Static);
}
