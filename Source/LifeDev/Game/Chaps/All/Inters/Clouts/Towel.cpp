// Copyright Jerónimo Barraco-Mármol

#include "Towel.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"

ATowel::ATowel():Super() {
	StateNum = 1;
	RewardFlash = 0;
	UseFade = true;
	Texts = { NSLOCTEXT("Towel", "State0", "Towel")};
	UseAnim = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Clouts/Towel00_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-7.500000,12.500000,0.000000));
	Mesh->SetCastAllShadows(true);

	Interact->SetRelativeLocation(FVector(7.500000,-12.500000,17.500000));
	Interact->SetBoxExtent(FVector(7.500000,12.500000,17.500000));
	// static ConstructorHelpers::FObjectFinder<USoundBase>
	// 	CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Towel00_P"));

	// TODO
	// SFXs = {CSnd.Object};
	
	Mesh->SetCastAllShadows(true);
	Super::SetMobility(EComponentMobility::Static);
}
