// Copyright Jerónimo Barraco-Mármol

#include "Fridge.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AFridge::AFridge():Super() {
	Texts = { FText::FromString(TEXT("Fridge")) } ;
	RewardFlash = 0;
	UseAnim = false;
	UseRewardFade = false;
	StateNum = 1;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Fridge.Fridge"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(60.5,17,0));
	
	Interact->SetRelativeLocation(FVector(-90,-12.5,90));
	Interact->SetBoxExtent(FVector(35,7.5,85));
	SFX->SetRelativeLocation(FVector(-90,-15,95));
	
	// stolen from paper. maybe get a new one?
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/Grab_C.Grab_C"));
	SFX_Trigger = CSnd.Object;

	// set static by default
	AFridge::SetMobility(EComponentMobility::Static);
	Mesh->SetQuickCollisionEnabled(true);
	// too big to not cast shadows. will look weird.
	Mesh->SetCastAllShadows(true);
}

