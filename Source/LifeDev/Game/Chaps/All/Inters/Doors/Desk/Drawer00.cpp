// Copyright Jerónimo Barraco-Mármol

#include "Drawer00.h"

#include "Components/AudioComponent.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ADrawer00::ADrawer00():Super() {
	UseAutoActivate = true;

	//  Set the anim root to the root component so that i can attach items to it
	SFX->SetRelativeLocation(FVector(80,-50,55));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Desk00/Desk00-Drawer.Desk00-Drawer"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-15.000000,30.000000,-50.000000));
	Interact->SetRelativeLocation(FVector(85.196155,-46.834350,55.659919));
	Interact->SetBoxExtent(FVector(5.000000,18.666945,5.000000));
	SFX->SetRelativeLocation(FVector(8.,0.,2.5));
}
