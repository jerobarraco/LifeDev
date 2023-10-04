// Copyright Jerónimo Barraco-Mármol

#include "Light00.h"

#include "Components/RectLightComponent.h"
#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ALight00::ALight00():Super() {
	Mesh->SetRelativeLocation(FVector(-2.5,2.5,0));
	Mesh->SetRelativeScale3D(FVector(0.05,0.05,0.05));

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
	RectLight->SetIntensity(300);

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
