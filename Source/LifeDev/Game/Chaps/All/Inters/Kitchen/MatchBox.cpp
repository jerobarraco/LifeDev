// Copyright Jerónimo Barraco-Mármol

#include "MatchBox.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AMatchBox::AMatchBox():Super() {
	RewardFlash = 0;
	UseAnim = false;
	StateNum = 1;
	// if you need it to fade, you'll need a new mat.
	UseFade = false; // not setting newmat because it won't fade.
	Texts = { NSLOCTEXT("MatchBox", "State0", "Match box") };

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/MatchBox/Matchbox00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-2.000000,2.750000,-0.500000));
	Mesh->SetUseDynShadow(true);
	Mesh->bUseAsOccluder = false;

	Interact->SetRelativeLocation(FVector(2.000000,-2.750000,0.750000));
	Interact->SetBoxExtent(FVector(4.000000,5.000000,1.500000));
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/PaperBox/KITCHEN_cardboard_box_manipulating-Cue"));
	// SFX_Trigger = CSnd.Object;
	SFXs = { CSnd.Object };

	AMatchBox::SetMobility(EComponentMobility::Static);
}
