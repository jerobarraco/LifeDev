// Copyright Jerónimo Barraco-Mármol

#include "Paper00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"

#include "JUtils/Actors/CQuickMesh.h"

APaper00::APaper00():Super() {
	UseAnim = false;
	UseFade = true;
	UseAutoActivate = true; // ok (and lazy) here since it's very unlikely papers show without being interactable.
	StateNum = 1;
	Texts = { NSLOCTEXT("Paper00", "State0", "A piece of paper") };
	// a lazy dog is a sheet of paper
	// a sheet of paper is an ink lined plane
	// an inclined plane is a slope up
	// a slow pup is a lazy dog

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Paper00/Paper00.Paper00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-7.5,10.5,0));
	Mesh->SetCastAllShadows(true);

	Interact->SetRelativeLocation(FVector(7.5,-10.5,0.25));
	Interact->SetBoxExtent(FVector(7.5,10.5,1));
	SFX->SetRelativeLocation(FVector(7.5,-10.5,0.25));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Paper00/Paper_SC.Paper_SC"));
	SFXTrigger = CSnd.Object;

	Super::SetMobility(EComponentMobility::Static);
}
