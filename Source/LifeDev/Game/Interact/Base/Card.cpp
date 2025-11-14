// Copyright Jerónimo Barraco-Mármol

#include "Card.h"

#include "JUtils/Actors/CQuickMesh.h"

#include "Interact/CInteract.h"

ACard::ACard():Super() {
	UseAnim = false;
	UseFade = true;
	UseRewardDestroy = true;
	RewardFlash = -.1;
	StateNum = 1;
	Texts = { NSLOCTEXT("Card", "State0", "A card") };
	UseAutoActivate = true;

	Interact->SetRelativeLocation(FVector(5.000000,-2.500000,0.000000));
	Interact->SetBoxExtent(FVector(10.000000,5.000000,5.000000));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Cards/Card00.Card00"));
	Mesh->SetStaticMesh(CMesh.Object);

	// stolen from paper. maybe get a new one?
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Paper00/Paper_SC.Paper_SC"));
	// SFXTrigger = CSnd.Object;
	SFXs = { CSnd.Object };

	ACard::SetMobility(EComponentMobility::Static);
}
