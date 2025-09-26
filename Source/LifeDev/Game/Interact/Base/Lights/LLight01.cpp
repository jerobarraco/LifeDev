// Copyright Jerónimo Barraco-Mármol

#include "LLight01.h"

#include "Components/RectLightComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ALLight01::ALLight01():Super() {
	UseAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Fluorescent/Support.Support"));
	Mesh->SetRelativeLocation(FVector(0,0,0));
	Mesh->SetRelativeRotation(FRotator(0,270,90));
	Mesh->SetRelativeScale3D(FVector(1,1,1));
	Mesh->SetStaticMesh(CMesh.Object);

	Interact->SetRelativeLocation(FVector(2.500000,-2.500000,0));
	Interact->SetBoxExtent(FVector(42.500000,7.500000,5.000000));
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve (TEXT("/Game/LifeDev/Game/Inters/Lights/Fluorescent/C_Fluorescent.C_Fluorescent"));
	if (LIKELY(CCurve.Succeeded())) Anim->Curve = CCurve.Object;

	RectLight = CreateDefaultSubobject<URectLightComponent>(TEXT("Light"));
	RectLight->SetupAttachment(Mesh);
	RectLight->SetRelativeLocation(FVector(2.500000,-2.500000,-0.200000));
	RectLight->SetRelativeRotation(FRotator(-90,-90,0));
	RectLight->SetIntensityUnits(ELightUnits::Lumens);
	IntensityMax = 20;
	RectLight->SetIntensity(IntensityMax);
	RectLight->SetAttenuationRadius(400);
	RectLight->SetSourceWidth(75);
	RectLight->SetSourceHeight(5);
	RectLight->SetBarnDoorAngle(90.010101);
	RectLight->SetBarnDoorLength(7);

	ALLight01::SetMobility(EComponentMobility::Static);
}

void ALLight01::SetMobility(const EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	RectLight->SetMobility(Mobility == EComponentMobility::Movable ? Mobility : EComponentMobility::Stationary);
}

void ALLight01::AnimUpdate_Implementation(const float P, const float A) {
	Super::AnimUpdate_Implementation(P, A);
	RectLight->SetHiddenInGame(A<.5);
}

