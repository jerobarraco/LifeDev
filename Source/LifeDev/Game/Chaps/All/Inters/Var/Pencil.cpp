// Copyright Jerónimo Barraco-Mármol

#include "Pencil.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

APencil::APencil():Super() {
	Texts = { FText::FromString(TEXT("Pencil")) };
	RewardFlash = 0;
	UseAnim = false;
	StateNum = 1;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Engine/BasicShapes/Cube"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-0.500000,-0.250000,0));
	Mesh->SetRelativeScale3D(FVector(0.100000,0.010000,0.010000));
	
	Interact->SetBoxExtent(FVector(60,75,75));
	
	// TODO stolen from paper. maybe get a new one?
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Paper00/Paper_SC.Paper_SC"));
	SFX_Trigger = CSnd.Object;

	/// Anims
	SetEnabled(false);
	APencil::SetMobility(EComponentMobility::Static);
	UseFade = true;
	AnimFade->SetNewMat();
}
