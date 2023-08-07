// Copyright Jerónimo Barraco-Mármol

#include "Window01.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"

AWindow01::AWindow01():Super() {
	SFX->SetRelativeLocation(FVector(11,57,42));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Window00/Window00-SideB.Window00-SideB"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,125,0));
	Interact->SetRelativeLocation(FVector(9,-95,60));
	Interact->SetBoxExtent(FVector(5,30,55));

	GlassA->SetRelativeLocation(FVector(0,-60,-70));
	GlassB->SetRelativeLocation(FVector(0,-60,-35));
	GlassC->SetRelativeLocation(FVector(0,-60,0));
}
