// Copyright Jerónimo Barraco-Mármol

#include "LLight00.h"

#include "Components/PointLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ALLight00::ALLight00() {
	UseAutoActivate = false;
	/// anim
	// enable the animation so that we get the update. but don't actually animate any material
	Anim->Mat = nullptr;
	Anim->SetComponentTickInterval(1.0/30); // optim. it's used for light and audio, can be lower. might help not invalidate vsm, even though my tests indicate that it doesn't.
	// Anim->MatVEnd = FLinearColor(1, 1, 1, 1);
	// Anim->MatVName = "Emissive";

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Mesh);
	Light->SetRelativeLocation(FVector(27.5,-27.5,9));
	Light->SetIntensityUnits(ELightUnits::Lumens);
	Light->SetIntensity(700); // a 75w approx
	Light->SetAttenuationRadius(500);
	Light->SetSourceRadius(5);
	Light->SetSoftSourceRadius(7);
	Light->SetTemperature(3333);
	
	ALLight00::SetMobility(EComponentMobility::Static);
	UseAnim = true; // important to do after setmobility since it will turn it off.
	// careful that the A_STROBE will disable later maybe
}

void ALLight00::BeginPlay() {
	Super::BeginPlay();
	IntensityMax = Light->Intensity;
}

void ALLight00::AnimUpdate_Implementation(const float Progress, const float Alpha) {
	Super::AnimUpdate_Implementation(Progress, Alpha);
	Light->SetIntensity(IntensityMax*Alpha);
	// .05 since i modulate the intensity, not on this one since it's a tungsten light
	const bool IsOn = Alpha >= .05;
	Light->SetVisibility(IsOn); // optimization
}

void ALLight00::SetMobility(const EComponentMobility::Type Mobility) {
	Super::SetMobility(Mobility);
	Light->SetMobility(Mobility == EComponentMobility::Movable ? Mobility : EComponentMobility::Stationary);
}

