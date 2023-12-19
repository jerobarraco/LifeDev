// Copyright Jerónimo Barraco-Mármol

#include "LLight00.h"

#include "Components/PointLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ALLight00::ALLight00():Super() {
	/// anim
	// enable the animation so that we get the update. but don't actually animate any material
	Anim->Mat = nullptr;
	// Anim->MatVEnd = FLinearColor(1, 1, 1, 1);
	// Anim->MatVName = "Emissive";
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve (TEXT("/JUtils/Curves/NoiseRamp_C.NoiseRamp_C"));
	if (CCurve.Succeeded()) {
		Anim->Curve = CCurve.Object;
	}

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Mesh);
	Light->SetRelativeLocation(FVector(27.5,-27.5,9));
	Light->SetIntensityUnits(ELightUnits::Lumens);
	Light->SetIntensity(IntensityMax);
	Light->SetAttenuationRadius(1000);
	Light->SetSourceRadius(10);
	Light->SetSoftSourceRadius(5);
	Light->SetTemperature(3333);
	
	ALLight00::SetMobility(EComponentMobility::Static);
	UseAnim = true; // important to do after setmobility since it will turn it off.
}

void ALLight00::AnimUpdate_Implementation(float Progress, float Alpha) {
	Light->SetIntensity(IntensityMax*Alpha);
	// .05 since i modulate the intensity not on this one since it's a tungsten light
	const bool IsOn = Alpha >= .05;
	Light->SetVisibility(IsOn); // optimization
}

void ALLight00::SetMobility(EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Light->SetMobility(Mobility == EComponentMobility::Movable ? Mobility : EComponentMobility::Stationary);
}

