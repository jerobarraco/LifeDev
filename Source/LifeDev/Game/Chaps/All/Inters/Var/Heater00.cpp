// Copyright Jerónimo Barraco-Mármol

#include "Heater00.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

AHeater00::AHeater00():Super() {
	SFX->SetRelativeLocation(FVector(9,0,35));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Heater00/Heater00.Heater00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(30,40,0));
	Mesh->SetRelativeRotation(FRotator(0,-90, 0));

	Interact->SetRelativeLocation(FVector(40.,-15.,47.5));
	Interact->SetBoxExtent(FVector(40,15,47.5));

	// stub animation but wont play
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/JUtils/Curves/BounceOut_C.BounceOut_C"));
	// leave the default in case of issues
	if (CCurve.Succeeded())
		Anim->Curve = CCurve.Object;
	Anim->SetComponentTickInterval(1/60.f);
	Anim->TEnd.SetRotation(FRotator(-10,0,0).Quaternion());
	Anim->Duration = .75f;
	Anim->IsAdditive = true;

	// disable by default
	Interact->SetEnabled(false);
	AnimEnabled = false;
}
