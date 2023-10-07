// Copyright Jerónimo Barraco-Mármol

#include "Light00.h"

#include "Components/RectLightComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ALight00::ALight00():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Fluorescent/Support.Support"));
	Mesh->SetRelativeLocation(FVector(-2.5,2.5,0));
	Mesh->SetRelativeScale3D(FVector(0.05,0.05,0.05));
	Mesh->SetStaticMesh(CMesh.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CTube (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Fluorescent/Fluorescent.Fluorescent"));
	// TODO tube position?
	Tube = CreateDefaultSubobject<UCQuickMesh>(TEXT("Tube"));
	Tube->SetupAttachment(Mesh);
	if (CTube.Succeeded()) {
		Tube->SetStaticMesh(CTube.Object);
	}
	Tube->SetRelativeLocation(FVector(0.5,7.5,100));

	RectLight = CreateDefaultSubobject<URectLightComponent>(TEXT("Light"));
	RectLight->SetupAttachment(Mesh);
	RectLight->SetRelativeLocation(FVector(0.5,7.5,100));
	RectLight->SetRelativeRotation(FRotator(90,90,0));
	RectLight->SetIntensityUnits(ELightUnits::Lumens);
	RectLight->SetIntensity(1.5);
	RectLight->SetAttenuationRadius(700);
	RectLight->SetSourceWidth(75);
	RectLight->SetSourceHeight(5);
	RectLight->SetBarnDoorAngle(90.000000);
	RectLight->SetBarnDoorLength(7);
	// RectLight->SetBarnDoorLength()
	// TODO light config

	Interact->SetRelativeLocation(FVector(40,-50,103.734790));
	Interact->SetBoxExtent(FVector(900,200,150));
	SetEnabled(false);
	// Texts = {FText::FromString("PickUp")};
	// static ConstructorHelpers::FObjectFinder<USoundBase>
		// CSnd (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Light00s/Light00s.Light00s"));
	// SFX_Trigger = CSnd.Object;
	// TriggerFlashInc = .1;
	ALight00::SetMobility(EComponentMobility::Static);
}

void ALight00::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Tube->SetMobility(Mobility);
	RectLight->SetMobility(Mobility == EComponentMobility::Movable ? Mobility : EComponentMobility::Stationary);
}
