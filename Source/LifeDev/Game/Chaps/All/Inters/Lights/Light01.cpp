// Copyright Jerónimo Barraco-Mármol

#include "Light01.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ALight01::ALight01():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Light00/Light00-Frame.Light00-Frame"));
	Mesh->SetRelativeLocation(FVector(-27.500000,27.500000,0.000000));
	Mesh->SetStaticMesh(CMesh.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CTube (TEXT("/Game/LifeDev/Game/Inters/Lights/Light00/Light00-GlassB.Light00-GlassB"));
	Glass = CreateDefaultSubobject<UCQuickMesh>(TEXT("Tube"));
	Glass->SetupAttachment(Mesh);
	Glass->SetStaticMesh(CTube.Object);
	Glass->SetRelativeLocation(FVector(5.,-5.,5.));
	Glass->SetCastAllShadows(false);

	Interact->SetRelativeLocation(FVector(27.500000,-27.500000,15));
	Interact->SetBoxExtent(FVector(28,28.5,15.5));

	ALight01::SetMobility(EComponentMobility::Static);
}

void ALight01::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Glass->SetMobility(Mobility);
}

