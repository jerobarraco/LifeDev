// Copyright Jerónimo Barraco-Mármol

#include "Phone00.h"

#include "CQuickMesh.h"
#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"

APhone00::APhone00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CM(TEXT("/Game/LifeDev/Game/Inters/Var/Phone00_P"));
	Mesh->SetStaticMesh(CM.Object);
	Interact->SetRelativeLocation(FVector(15.,-15,5));
	Interact->SetBoxExtent(FVector(15, 15, 5));
	SFX->SetRelativeLocation(FVector(0,0, 5));
	SetMobility(EComponentMobility::Static);
}
