// Copyright Jerónimo Barraco-Mármol

#include "Screwer00.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AScrewer00::AScrewer00():Super() {
	Texts = { NSLOCTEXT("Screwer00", "State0", "Screw-driver") };
	RewardFlash = 0;
	UseAnim = false;
	StateNum = 1;
	// TODO to the instance, to the wow
	RewardItem = "Screwer00";
	UseFade = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Var/Screwer_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,-0.50000,0));
	Mesh->SetRelativeRotation(FRotator(0,0,0));
	
	Interact->SetRelativeLocation(FVector(0,0,7));
	Interact->SetBoxExtent(FVector(5,5,10));
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT(""));
	SFXs = {CSnd.Object};

	AScrewer00::SetMobility(EComponentMobility::Static);
}
// todo make a screweri00 and replace on outliner