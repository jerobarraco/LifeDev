// Copyright Jerónimo Barraco-Mármol

#include "Screwer00.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AScrewer00::AScrewer00():Super() {
	Texts = { NSLOCTEXT("Screwer00", "State0", "Screw-driver") };
	RewardFlash = 0;
	UseAnim = false;
	StateNum = 1;
	UseFade = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Var/Screwer_SP"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(.000000,2.500000,-2.500000));

	Interact->SetRelativeLocation(FVector(12.500000,-2.500000,2.500000));
	Interact->SetBoxExtent(FVector(12.500000,2.500000,2.500000));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/Rotation_squeak"));
	SFXs = {CSnd.Object};

	AScrewer00::SetMobility(EComponentMobility::Static);
}
