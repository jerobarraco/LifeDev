// Copyright Jerónimo Barraco-Mármol

#include "Light02.h"

#include "Components/AudioComponent.h"
#include "Components/PointLightComponent.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"
#include "LifeDev/Core/Sounds/CLSounder.h"

ALight02::ALight02():Super() {
	UseAnim = true;
	UseAutoActivate = true; // this one is ok to interact with

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
	// important: in order for the shadows to work correctly at this short distance
	// from the source. the mesh needs to use the translucent material.
	// to use the translucent material, it needs to not be nanite.
	Cover->bDisallowNanite = true;
	Cover->SetCastAllShadows(true);
	Cover->bCastDistanceFieldIndirectShadow = true;

	Light->SetRelativeLocation(FVector(20,-20,160));
	Light->Intensity = 5;
	Light->AttenuationRadius = 500;
	Light->SourceRadius = 1;
	Light->SourceLength = 1;
	Light->SoftSourceRadius = 20;
	
	Interact->SetRelativeLocation(FVector(20,-20,90));
	Interact->SetBoxExtent(FVector(20,20,90));

	const FVector SndLoc(15,-20,145);
	SFX->SetRelativeLocation(SndLoc);
	SFX_Flicker->SetRelativeLocation(SndLoc);

	ALight02::SetMobility(EComponentMobility::Static);
}

void ALight02::SetMobility(const EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Cover->SetMobility(Mobility);
}

