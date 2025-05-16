// Copyright Jerónimo Barraco-Mármol

#include "FoodBox.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AFoodBox::AFoodBox():Super() {
	Texts = { NSLOCTEXT("FoodBox", "State0", "Food box") };
	RewardFlash = 0;
	UseRewardDestroy = true;
	UseFade = true;
	StateNum = 1;
	UseAnim = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Box_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));
	Mesh->SetCastAllShadows(true);
	// not bothering with the material for now, i prefer not to load 2 materials.
	// would have to be set on child instances.
	
	Interact->SetRelativeLocation(FVector(10,-5,12.5));
	Interact->SetBoxExtent(FVector(15,7.5,15));
	SFX->SetRelativeLocation(FVector(10,-5,12.5));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/PaperBox/KITCHEN_cardboard_box_manipulating-Cue"));
	SFXs = { CSnd.Object };
	Super::SetMobility(EComponentMobility::Static);
}
