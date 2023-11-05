// Copyright Jerónimo Barraco-Mármol

#include "Radio00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ARadio00::ARadio00():Super() {
	SFX->SetRelativeLocation(FVector(15,0,15));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Radio00/Radio00_Door.Radio00_Door"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,30,0));
	// Mesh->SetRelativeRotation(FRotator(0,-90, 0));

	Interact->SetRelativeLocation(FVector(40.,-15.,47.5));
	Interact->SetBoxExtent(FVector(40,15,47.5));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CBase(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Radio00/Radio00_Base.Radio00_Base"));
	Base = CreateDefaultSubobject<UCQuickMesh>(TEXT("Base"));
	Base->SetupAttachment(RootComponent);
	Base->SetStaticMesh(CBase.Object);
	Base->SetRelativeLocation(FVector(0,30,0));
	

	// disable by default
	Interact->SetEnabled(false);
	AnimEnabled = false;
}
