// Copyright Jerónimo Barraco-Mármol

#include "PianoKey.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

APianoKey::APianoKey():Super() {
	Texts = { NSLOCTEXT("PianoKey", "State0", "Play") };
	StateNum = 1;
	UseAnim = true;
	IsOneShot = true; // will disable the keys on trigger. will be re-enabled on puzzle reset.
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Group_A"));
	// SFX_Trigger = CSFX.Object;
	SFXs = { CSFX.Object };

	Mesh->SetUseDynShadow(true);
	Interact->SetRelativeLocation(FVector(0,7.5,-2.5));
	Interact->SetBoxExtent(FVector(12.5,7.5,2.5)); // this is the most common

	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/JUtils/Curves/PulseOut.PulseOut"));
	// niagara curves are not loaded somehow
	Anim->Curve = CCurve.Object;
	Anim->Duration = 1;

	Anim->IsAdditive = true;
	Anim->TEnd.SetRotation(FRotator(0,0,10).Quaternion());
}
