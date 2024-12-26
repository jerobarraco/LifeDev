// Copyright Jerónimo Barraco-Mármol

#include "MatchBox.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AMatchBox::AMatchBox():Super() {
	Texts = { FText::FromString(TEXT("Match box")) };
	RewardFlash = 0;
	UseAnim = false;
	StateNum = 1;
	UseFade = false; // not setting newmat because it won't fade.
	// if you need it to fade, you'll need a new mat.

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/MatchBox/Matchbox00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-2.000000,2.750000,-0.500000));
	
	Interact->SetRelativeLocation(FVector(2.000000,-2.750000,0.750000));
	Interact->SetBoxExtent(FVector(4.000000,5.000000,1.500000));
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/PaperBox/KITCHEN_cardboard_box_manipulating-Cue"));
	SFX_Trigger = CSnd.Object;

	Super::SetAutoActivate(true);
	AMatchBox::SetMobility(EComponentMobility::Static);
}
