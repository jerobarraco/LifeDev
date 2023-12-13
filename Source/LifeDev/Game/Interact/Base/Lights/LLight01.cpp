// Copyright Jerónimo Barraco-Mármol

#include "LLight01.h"

#include "Components/RectLightComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ALLight01::ALLight01():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Fluorescent/Support.Support"));
	Mesh->SetRelativeLocation(FVector(-2.5,2.5,0));
	Mesh->SetRelativeScale3D(FVector(0.05,0.05,0.05));
	Mesh->SetStaticMesh(CMesh.Object);
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve (TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Fluorescent/C_Fluorescent.C_Fluorescent"));
	if (CCurve.Succeeded()) {
		Anim->Curve = CCurve.Object;
	}

	RectLight = CreateDefaultSubobject<URectLightComponent>(TEXT("Light"));
	RectLight->SetupAttachment(Mesh);
	RectLight->SetRelativeLocation(FVector(0.5,7.5,101));
	RectLight->SetRelativeRotation(FRotator(90,90,0));
	RectLight->SetIntensityUnits(ELightUnits::Lumens);
	RectLight->SetIntensity(1.5);
	RectLight->SetAttenuationRadius(701);
	RectLight->SetSourceWidth(75);
	RectLight->SetSourceHeight(5);
	RectLight->SetBarnDoorAngle(90.010101);
	RectLight->SetBarnDoorLength(7);

	ALLight01::SetMobility(EComponentMobility::Static);
}

void ALLight01::UpdateAnim(float Progress, float Alpha) {
	const bool IsOn = Alpha >= .45;
	RectLight->SetVisibility(IsOn);
}

void ALLight01::BeginPlay() {
	Super::BeginPlay();
	Anim->OnUpdate.AddUniqueDynamic(this, &ALLight01::UpdateAnim);
}

void ALLight01::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Anim->OnUpdate.RemoveAll(this);
	Anim->Mat = nullptr;
	Super::EndPlay(EndPlayReason);
}

void ALLight01::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	RectLight->SetMobility(Mobility == EComponentMobility::Movable ? Mobility : EComponentMobility::Stationary);
}

