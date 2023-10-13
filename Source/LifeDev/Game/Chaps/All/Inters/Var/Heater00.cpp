// Copyright Jerónimo Barraco-Mármol

#include "Heater00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AHeater00::AHeater00():Super() {
	SFX->SetRelativeLocation(FVector(9,0,35));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Heater00/Heater00.Heater00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(30,40,0));
	Mesh->SetRelativeRotation(FRotator(0,-90, 0));

	Interact->SetRelativeLocation(FVector(40.,-15.,47.5));
	Interact->SetBoxExtent(FVector(40,15,47.5));

	// disable by default
	Interact->SetEnabled(false);
	AnimEnabled = false;
}
