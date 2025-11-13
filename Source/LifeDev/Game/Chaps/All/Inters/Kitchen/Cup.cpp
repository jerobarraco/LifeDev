// Copyright Jerónimo Barraco-Mármol

#include "Cup.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ACup::ACup():Super() {
	Texts = { NSLOCTEXT("Cup", "State0", "A cup") };
	RewardFlash = 0;
	UseRewardDestroy = true;
	UseFade = false;
	StateNum = 1;
	UseAnim = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Utensils/Cup00_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-5.50,5.250000,0.));
	Mesh->SetUseDynShadow(true);

	Interact->SetRelativeLocation(FVector(5.5,-5.25,5.25));
	Interact->SetBoxExtent(FVector(5.750000,5.500000,5.500000));

	// TODO
	// static ConstructorHelpers::FObjectFinder<USoundBase>
		// CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/PaperBox/KITCHEN_cardboard_box_manipulating-Cue"));
	// SFXs = { CSnd.Object };
	Super::SetMobility(EComponentMobility::Static);
}
