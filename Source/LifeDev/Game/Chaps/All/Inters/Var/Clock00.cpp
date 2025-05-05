// Copyright Jerónimo Barraco-Mármol

#include "Clock00.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AClock00::AClock00():Super() {
	Texts = { NSLOCTEXT("Clock00", "State0", "Alarm clock") };
	RewardFlash = 0;
	UseAnim = false;
	StateNum = 1;
	UseFade = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Var/Clock_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(.000000,0,0));

	Interact->SetRelativeLocation(FVector(15.00000,-7.500000,10.00000));
	Interact->SetBoxExtent(FVector(15.00000,7.500000,10.00000));

	// TODO
	// i'd get a compiler error on windows if it can't find it
	// static ConstructorHelpers::FObjectFinder<USoundBase>
		// CSnd(TEXT(""));
	// SFXs = {CSnd.Object};

	AClock00::SetMobility(EComponentMobility::Static);
}
