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
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Arch/Neighborhood/StreetLight01.StreetLight01"));
	Mesh->SetRelativeLocation(FVector(-17.5,17.5,0));
	Mesh->SetStaticMesh(CMesh.Object);
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
			CCurve (TEXT("/JUtils/Curves/Noise_C.Noise_C"));
	if (CCurve.Succeeded()) {
		Anim->Curve = CCurve.Object;
	}
	
	Interact->SetRelativeLocation(FVector(17.5,-17.5,75));
	Interact->SetBoxExtent(FVector(17.5,17.5,75));

	SFX->SetRelativeLocation(FVector(17.5,-17.5,100));
	Light->SetRelativeLocation(FVector(17.5,-160,330));
	Light->SetRelativeRotation(FRotator(-90,0,0));
	
	static const FColor Col(
		LDConsts::Colors::Palette[LDConsts::Colors::Brown][10]);
	Light->SetLightColor(Col, true);
	Light->SetAttenuationRadius(500);
	Light->SetInnerConeAngle(10);
	Light->SetOuterConeAngle(40);

	Rnd->DelayMin = 10;
	Rnd->DelayMax = 50;
	Rnd->ValueMin = 1;
	Rnd->ValueMax = 5;
	Rnd->UseAnimRandReverse = true;

	FlickrOnFB = .2; // always flicker

	Super::SetAutoActivate(false); // by default, you should not be able to turn a streetlight off
}