// Copyright Jerónimo Barraco-Mármol

#include "Light02.h"

#include "Components/AudioComponent.h"
#include "Components/PointLightComponent.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

ALight02::ALight02():Super() {
	UseAnim = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Lamp00/Lamp00_Stand.Lamp00_Stand"));
	Mesh->SetRelativeLocation(FVector(-20,20,0));
	Mesh->SetStaticMesh(CMesh.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CTube (TEXT("/Game/LifeDev/Game/Inters/Lights/Lamp00/Lamp00_Cover.Lamp00_Cover"));
	Cover = CreateDefaultSubobject<UCQuickMesh>(TEXT("Cover"));
	Cover->SetupAttachment(Mesh);
	Cover->SetStaticMesh(CTube.Object);
	Cover->SetRelativeLocation(FVector(0,0,0));
	Cover->SetCastAllShadows(true);
	Cover->bCastDistanceFieldIndirectShadow = true;

	Light->SetRelativeLocation(FVector(20,-20,160));
	
	Interact->SetRelativeLocation(FVector(20,-20,90));
	Interact->SetBoxExtent(FVector(20,20,90));

	SFX->SetRelativeLocation(FVector(15,-20,145));

	ALight02::SetMobility(EComponentMobility::Static);
	SetEnabled(true); // this one is ok to toggle
}

void ALight02::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Cover->SetMobility(Mobility);
}

