// Copyright Jerónimo Barraco-Mármol

#include "PianoKey.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

APianoKey::APianoKey():Super() {
	Texts = {FText::FromString("Play")};
	StateNum = 1;
	UseAnim = true;
	
	// TODO change on sfx days
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX (TEXT("/Game/LifeDev/Game/Inters/Key00/Key19.Key19"));
	SFX_Trigger = CSFX.Object;
	
	Interact->SetRelativeLocation(FVector(0,7.5,-2.5));
	Interact->SetBoxExtent(FVector(12.5,7.5,2.5)); // this is the most common


	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve (TEXT("/Niagara/DefaultAssets/Curves/Templates/RampUpDown.RampUpDownF"));
	Anim->Curve = CCurve.Object;
	Anim->Duration = .3;
	Anim->TEnd.SetRotation(FRotator(0,0,10).Quaternion());
}
