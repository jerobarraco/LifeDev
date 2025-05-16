// Copyright Jerónimo Barraco-Mármol

#include "Pencil.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

APencil::APencil():Super() {
	Texts = { NSLOCTEXT("Pencil", "State0", "Pencil") };
	RewardFlash = 0;
	UseAnim = false;
	StateNum = 1;
	UseFade = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Var/Pencil_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0.750000,-0.750000));
	Mesh->SetCastAllShadows(true);

	Interact->SetBoxExtent(FVector(7.5,1,1));
	Interact->SetRelativeLocation(FVector(7,-0.750000,0.750000));
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Paper00/Paper_SC"));
	// SFXTrigger = CSnd.Object;
	SFXs = { CSnd.Object };

	/// Anims
	// AnimFade->SetNewMat();

	APencil::SetMobility(EComponentMobility::Static);
}
