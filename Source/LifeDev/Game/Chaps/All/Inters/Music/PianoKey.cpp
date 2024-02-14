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

	// static ConstructorHelpers::FObjectFinder<UCurveFloat>
		// CCurve (TEXT("/Niagara/DefaultAssets/Curves/Templates/RampUpDown.RampUpDownF"));
	// niagara curves are not loaded somehow
	Anim->Curve = nullptr;
	Anim->Duration = 1;
	Anim->IsBouncing = true; // TODO bouncing gets reset
	Anim->TEnd.SetRotation(FRotator(0,0,10).Quaternion());
}
// todo improve animations

void APianoKey::SetState_Implementation(int32 NewState) {
	// Anim->IsReversed = true;
	Anim->IsBouncing = true;
	Super::SetState_Implementation(NewState);
}

void APianoKey::PostInitializeComponents() {
	Super::PostInitializeComponents();
	// UCodeCurveLib* const CurveLib = NewObject<UCodeCurveLib>(this);
	UCodeCurveLib* const CurveLib = Cast<UCodeCurveLib>(UCodeCurveLib::StaticClass()->GetDefaultObject(true));
	Anim->CodeCurve.BindDynamic(CurveLib, &UCodeCurveLib::OutBack);
}
