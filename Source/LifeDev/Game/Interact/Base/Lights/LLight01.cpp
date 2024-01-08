// Copyright Jerónimo Barraco-Mármol

#include "LLight01.h"

#include "Components/RectLightComponent.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ALLight01::ALLight01():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Fluorescent/Support.Support"));
	Mesh->SetRelativeLocation(FVector(-2.5,2.5,0));
	Mesh->SetRelativeScale3D(FVector(0.05,0.05,0.05));
	Mesh->SetStaticMesh(CMesh.Object);
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve (TEXT("/Game/LifeDev/Game/Inters/Lights/Fluorescent/C_Fluorescent.C_Fluorescent"));
	if (CCurve.Succeeded()) {
		Anim->Curve = CCurve.Object;
	}

	RectLight = CreateDefaultSubobject<URectLightComponent>(TEXT("Light"));
	RectLight->SetupAttachment(Mesh);
	RectLight->SetRelativeLocation(FVector(0.5,7.5,101));
	RectLight->SetRelativeRotation(FRotator(90,90,0));
	RectLight->SetIntensityUnits(ELightUnits::Lumens);
	RectLight->SetIntensity(1);
	RectLight->SetAttenuationRadius(1000);
	RectLight->SetSourceWidth(75);
	RectLight->SetSourceHeight(5);
	RectLight->SetBarnDoorAngle(90.010101);
	RectLight->SetBarnDoorLength(7);

	ALLight01::SetMobility(EComponentMobility::Static);
}

void ALLight01::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	RectLight->SetMobility(Mobility == EComponentMobility::Movable ? Mobility : EComponentMobility::Stationary);
}

void ALLight01::AnimUpdate_Implementation(float P, float A) {
	Super::AnimUpdate_Implementation(P, A);
	RectLight->SetHiddenInGame(A<.5);
}

