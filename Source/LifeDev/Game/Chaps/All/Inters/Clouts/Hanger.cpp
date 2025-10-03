// Copyright Jerónimo Barraco-Mármol

#include "Hanger.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"

AHanger::AHanger():Super() {
	StateNum = 1;
	RewardFlash = 0;
	Texts = { NSLOCTEXT("Hanger", "State0", "Hanger")};
	UseAnim = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Clouts/Hanger/Hanger_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-7.500000,12.500000,-2.500000));
	Mesh->SetCastAllShadows(true);

	Interact->SetRelativeLocation(FVector(17.500000,-22.500000,2.500000));
	Interact->SetBoxExtent(FVector(17.500000,22.500000,2.500000));
	// TODO
	// static ConstructorHelpers::FObjectFinder<USoundBase>
	// 	CSnd (TEXT("/Game/LifeDev/Game/Inters/Clouts/Hanger00_P"));
	// SFXs = {CSnd.Object};
	
	Mesh->SetCastAllShadows(true);
	Super::SetMobility(EComponentMobility::Static);
}
