// Copyright Jerónimo Barraco-Mármol

#include "Cup.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ACup::ACup():Super() {
	Texts = { NSLOCTEXT("Cup", "State0", "A cup") };
	RewardFlash = 0;
	UseRewardDestroy = true;
	UseFade = true;
	StateNum = 1;
	UseAnim = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Utensils/Cup00_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	// TQDQ
	Mesh->SetRelativeLocation(FVector(0,0,0));
	Mesh->SetCastAllShadows(true);

	// TODO
	Interact->SetRelativeLocation(FVector(10,-5,12.5));
	Interact->SetBoxExtent(FVector(15,7.5,15));
	SFX->SetRelativeLocation(FVector(10,-5,12.5));

	// TODO
	// static ConstructorHelpers::FObjectFinder<USoundBase>
		// CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/PaperBox/KITCHEN_cardboard_box_manipulating-Cue"));
	// SFXs = { CSnd.Object };
	Super::SetMobility(EComponentMobility::Static);
}
