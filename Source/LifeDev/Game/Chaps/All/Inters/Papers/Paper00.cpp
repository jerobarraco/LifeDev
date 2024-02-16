// Copyright Jerónimo Barraco-Mármol

#include "Paper00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

APaper00::APaper00():Super() {
	UseAnim = false;
	StateNum = 1;
	Texts = { FText::FromString(TEXT("A piece of paper")) };
	// a lazy dog is a sheet of paper
	// a sheet of paper is an ink lined plane
	// an inclined plane is a slope up
	// a slow pup is a lazy dog

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Paper00/Paper00.Paper00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-7.5,10.5,0));
	
	Interact->SetRelativeLocation(FVector(7.5,-10.5,0.25));
	Interact->SetBoxExtent(FVector(7.5,10.5,1));
	SFX->SetRelativeLocation(FVector(7.5,-10.5,0.25));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Paper00/Paper_SC.Paper_SC"));
	SFX_Trigger = CSnd.Object;

	AnimFade->SetNewMat();
	Super::SetMobility(EComponentMobility::Static);
}
