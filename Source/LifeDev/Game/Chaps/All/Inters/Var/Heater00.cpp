// Copyright Jerónimo Barraco-Mármol

#include "Heater00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AHeater00::AHeater00():Super() {
	UseAnim = false;
	UseRewardDestroy = false;
	UseFade = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Heater00/Heater00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(30,40,0));
	Mesh->SetRelativeRotation(FRotator(0,-90, 0));
	Mesh->SetUseDynShadow(true);

	Interact->SetRelativeLocation(FVector(40.,-15.,47.5));
	Interact->SetBoxExtent(FVector(40,15,47.5));

	SFX->SetRelativeLocation(FVector(0,20,35));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Inters/Rooms/Heater/OLD_STUFF_Water_Heater_01-start"));
	SFXs = { CSFX.Object, CSFX.Object};
	SFXLocked = CSFX.Object;
}
