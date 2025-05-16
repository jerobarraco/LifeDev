// Copyright Jerónimo Barraco-Mármol

#include "Stove.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AStove::AStove():Super() {
	Texts = { NSLOCTEXT("Stove", "State0", "Stove") };
	RewardFlash = 0;
	UseAnim = false;
	UseRewardDestroy = false;
	StateNum = 1;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Stove.Stove"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0.5,45,0));
	Mesh->SetQuickCollisionEnabled(true);
	Mesh->SetCastAllShadows(true);

	Interact->SetRelativeLocation(FVector(37.5,-42.5,55));
	Interact->SetBoxExtent(FVector(37.5,42.5,55));
	SFX->SetRelativeLocation(FVector(37.5,-42.5,42.5));

	// TODO new one
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/Grab_C.Grab_C"));
	// SFX_Trigger = CSnd.Object;
	SFXs = { CSnd.Object };

	// set static by default
	AStove::SetMobility(EComponentMobility::Static);
}
