// Copyright Jerónimo Barraco-Mármol

#include "Card.h"

#include "JUtils/Actors/CQuickMesh.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"

ACard::ACard():Super() {
	Interact->SetRelativeLocation(FVector(5.000000,-2.500000,0.000000));
	Interact->SetBoxExtent(FVector(10.000000,5.000000,5.000000));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Card00/Card00.Card00"));
	Mesh->SetStaticMesh(CMesh.Object);

	// stolen from paper. maybe get a new one?
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Paper00/Paper_SC.Paper_SC"));
	SFX_Trigger = CSnd.Object;
	
	Texts = { FText::FromString(TEXT("A card")) };
	StateNum = 1;
	UseAnim = false;
	RewardFlash = -.1;
	UseRewardDestroy = true;
	UseFade = true;
	AnimFade->SetNewMat();
}
