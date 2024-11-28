// Copyright Jerónimo Barraco-Mármol

#include "PianoKey.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

APianoKey::APianoKey():Super() {
	Texts = {FText::FromString("Play")};
	StateNum = 1;
	UseAnim = true;
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Inters/Music/Piano/group_A.group_A"));
	SFX_Trigger = CSFX.Object;

	Mesh->SetCastAllShadows(true);
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
