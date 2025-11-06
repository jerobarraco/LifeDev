// Copyright Jerónimo Barraco-Mármol

#include "LLight02.h"

#include "Components/LightComponent.h"
#include "Components/SpotLightComponent.h"

#include "JUtils/Actors/CQuickMesh.h"
#include "LifeDev/Core/Consts/ConstColors.h"

ALLight02::ALLight02():Super() {
	IntensityMax = 10;
	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Mesh);
	Light->SetIntensityUnits(ELightUnits::Lumens);
	Light->SetIntensity(IntensityMax);
	static const FColor Col(
		LDConsts::Colors::Palette[LDConsts::Colors::Brown][10]);
	// static constexpr FColor Col(207, 169, 134, 255);
	Light->SetLightColor(Col, true);
	Light->SetAttenuationRadius(500);
	Light->SetInnerConeAngle(10);
	Light->SetOuterConeAngle(40);
	Light->SetUseRayTracedDistanceFieldShadows(true);

	ALLight02::SetMobility(EComponentMobility::Static);
	// UseAnim = true; // restore anim set by Super::SetMobility // not necessary anymore. delete.
}

void ALLight02::SetMobility(const EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Light->SetMobility(Mobility == EComponentMobility::Movable ? Mobility : EComponentMobility::Stationary);
}

void ALLight02::AnimUpdate_Implementation(const float P, const float A) {
	Super::AnimUpdate_Implementation(P, A);
	const bool IsOn = A >= .05;
	// Light->SetVisibility(IsOn); // optimization
	Light->SetHiddenInGame(!IsOn); // optimization
	Light->SetIntensity(IntensityMax*A);
}

