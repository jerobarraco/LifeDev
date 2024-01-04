// Copyright Jerónimo Barraco-Mármol

#include "StreetLight.h"

#include "Components/SpotLightComponent.h"
#include "Interact/Animator/CRandomizer.h"
#include "LifeDev/Game/Sys/Consts/ConstColors.h"

AStreetLight::AStreetLight():Super() {
	// TODO
	// static ConstructorHelpers::FObjectFinder<UStaticMesh>
	// 	CMesh (TEXT("/Game/LifeDev/Game/Inters/Fluorescent/Support.Support"));
	// Mesh->SetRelativeLocation(FVector(-2.5,2.5,0));
	// Mesh->SetRelativeScale3D(FVector(0.05,0.05,0.05));
	// Mesh->SetStaticMesh(CMesh.Object);
	
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

	FlickrOnFB = 0; // always flicker
}