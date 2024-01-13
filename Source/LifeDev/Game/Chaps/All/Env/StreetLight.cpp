// Copyright Jerónimo Barraco-Mármol

#include "StreetLight.h"

#include "CQuickMesh.h"
#include "Components/SpotLightComponent.h"
#include "Interact/Animator/CRandomizer.h"
#include "LifeDev/Game/Sys/Consts/ConstColors.h"

AStreetLight::AStreetLight():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Arch/Neighborhood/StreetLight01.StreetLight01"));
	Mesh->SetRelativeLocation(FVector(-17.5,17.5,0));
	Mesh->SetStaticMesh(CMesh.Object);

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
	Rnd->UseRandReverse = true;

	FlickrOnFB = .2; // always flicker
}