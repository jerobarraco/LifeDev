// Copyright Jerónimo Barraco-Mármol

#include "Keys00.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AKeys00::AKeys00():Super() {
	Texts = { FText::FromString(TEXT("Keys")) } ;
	RewardFlash = 0;
	UseAnim = false;
	StateNum = 1;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Key00/Key00.Key00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-0.500000,-0.250000,0));
	Mesh->SetRelativeRotation(FRotator(0,90,0));
	// TODO fix this by reimporting correctly and setting the scale on import
	Mesh->SetRelativeScale3D(FVector(0.005000));
	
	Interact->SetRelativeLocation(FVector(50,-100,1350));
	Interact->SetBoxExtent(FVector(650,200,1500));
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Key00/Key19.Key19"));
	SFX_Trigger = CSnd.Object;

	/// Anims
	SetEnabled(false);
	AKeys00::SetMobility(EComponentMobility::Static);
}
