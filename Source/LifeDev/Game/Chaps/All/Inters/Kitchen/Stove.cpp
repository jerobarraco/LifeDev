// Copyright Jerónimo Barraco-Mármol

#include "Stove.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AStove::AStove():Super() {
	Texts = { FText::FromString(TEXT("Stove")) } ;
	RewardFlash = 0;
	UseAnim = false;
	UseRewardFade = false;
	StateNum = 1;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Stove.Stove"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0.5,45,0));
	
	Interact->SetRelativeLocation(FVector(42.5,-42.5,55));
	Interact->SetBoxExtent(FVector(45,45,60));
	SFX->SetRelativeLocation(FVector(42.5,-42.5,55));

	// TODO new one
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/Grab_C.Grab_C"));
	SFX_Trigger = CSnd.Object;

	// set static by default
	AStove::SetMobility(EComponentMobility::Static);
}

