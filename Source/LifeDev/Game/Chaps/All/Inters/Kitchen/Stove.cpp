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
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Stove"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0.5,45,0));
	Mesh->SetQuickCollisionEnabled(true);
	Mesh->SetUseDynShadow(true);

	Interact->SetRelativeLocation(FVector(37.5,-42.5,45));
	// made less-tall so that it's easier to get the matches and not confuse with the pot.
	Interact->SetBoxExtent(FVector(37.5,42.5,40));
	SFX->SetRelativeLocation(FVector(37.5,-42.5,42.5));

	// TODO new one
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/Grab_C.Grab_C"));
	// SFX_Trigger = CSnd.Object;
	SFXs = { CSnd.Object };

	// set static by default
	AStove::SetMobility(EComponentMobility::Static);
}
