// Copyright Jerónimo Barraco-Mármol

#include "StreetLight.h"

#include "CQuickMesh.h"
#include "Components/AudioComponent.h"
#include "Components/SpotLightComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Interact/Animator/CRandomizer.h"
#include "LifeDev/Core/Consts/ConstColors.h"

AStreetLight::AStreetLight():Super() {
	FlickrOnFB = .2; // always flicker
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Arch/Neighborhood/StreetLight01.StreetLight01"));
	Mesh->SetRelativeLocation(FVector(-17.5,17.5,0));
	Mesh->SetStaticMesh(CMesh.Object);
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
			CCurve (TEXT("/JUtils/Curves/Noise_C.Noise_C"));
	if (CCurve.Succeeded())
		Anim->Curve = CCurve.Object;
	
	Interact->SetRelativeLocation(FVector(17.5,-17.5,75));
	Interact->SetBoxExtent(FVector(17.5,17.5,75));

	SFX->SetRelativeLocation(FVector(17.5,-17.5,100));
	Light->SetRelativeLocation(FVector(17.5,-160,330));
	Light->SetRelativeRotation(FRotator(-90,0,0));
	
	static const FColor Col(
		LDConsts::Colors::Palette[LDConsts::Colors::Brown][10]);
	Light->SetLightColor(Col, true);
	Light->SetInnerConeAngle(50);
	Light->SetOuterConeAngle(70);
	Light->SetAttenuationRadius(700);
	Light->SetIntensityUnits(ELightUnits::Lumens);
	Light->SetIntensity(9001);
	Light->SetCastVolumetricShadow(true);

	Rnd->DelayMin = 10;
	Rnd->DelayMax = 50;
	Rnd->ValueMin = 1;
	Rnd->ValueMax = 5;
	Rnd->UseAnimRandReverse = true;
}