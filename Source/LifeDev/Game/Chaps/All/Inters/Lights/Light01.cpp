// Copyright Jerónimo Barraco-Mármol

#include "Light01.h"

#include "Components/AudioComponent.h"
#include "Components/PointLightComponent.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ALight01::ALight01():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Light00/Light00-Frame.Light00-Frame"));
	Mesh->SetRelativeLocation(FVector(-27.500000,27.500000,0.000000));
	Mesh->SetStaticMesh(CMesh.Object);
	SFX->SetRelativeLocation(FVector(27.5,-27.5,0));

	Light->SetRelativeLocation(FVector(27.500000,-27.5,50));
	Light->AttenuationRadius = 500;
	Light->SetIntensity(700);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CTube (TEXT("/Game/LifeDev/Game/Inters/Lights/Light00/Light00-GlassB"));
	Glass = CreateDefaultSubobject<UCQuickMesh>(TEXT("Tube"));
	Glass->SetupAttachment(Mesh);
	Glass->SetStaticMesh(CTube.Object);
	Glass->SetRelativeLocation(FVector(5.,-5.,5.));
	// important since by default it uses the masked material (and nanite).
	// the masked material doesn't work well with shadows so close to the light source
	// and nanite does not support translucent.
	Glass->SetUseDynShadow(false);

	Interact->SetRelativeLocation(FVector(27.5,-27.5,15));
	Interact->SetBoxExtent(FVector(28,28.5,15.5));

	Anim->Duration = .25; // it's a bulb.
	ALight01::SetMobility(EComponentMobility::Static);
}

void ALight01::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Glass->SetMobility(Mobility);
}

