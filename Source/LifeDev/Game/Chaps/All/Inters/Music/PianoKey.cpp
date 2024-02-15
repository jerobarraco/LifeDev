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
		CSFX (TEXT("/Game/LifeDev/Game/Inters/Music/Piano/group_A.group_A"));
	SFX_Trigger = CSFX.Object;
	
	Interact->SetRelativeLocation(FVector(0,7.5,-2.5));
	Interact->SetBoxExtent(FVector(12.5,7.5,2.5)); // this is the most common

	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/JUtils/Curves/PulseOut.PulseOut"));
	// niagara curves are not loaded somehow
	Anim->Curve = CCurve.Object;
	Anim->Duration = 1;
	// this will avoid anim being set to reverse.
	// Trans.Add(FTransform(FRotator(0,0,10).Quaternion(), FVector::ZeroVector, FVector::Zero()));

	// the animation needs to play in a different way than what i want, hence i need to define tstart instead
	// Anim->IsAdditive = false;
	// Anim->TEnd = FTransform (FQuat::Identity, FVector::ZeroVector, FVector::OneVector); 
	// Anim->TStart = Anim->TEnd;
	Anim->IsAdditive = true;
	Anim->TEnd.SetRotation(FRotator(0,0,10).Quaternion());
}

// void APianoKey::SetState_Implementation(int32 NewState) {
	// overriden by AnimPlay and SetState
	// when using Trans, i need to reset since bouncing will leave it on true
	// Anim->IsReversed = false;
	// Anim->IsBouncing = true;
	// Super::SetState_Implementation(NewState);
// }

// void APianoKey::PostInitializeComponents() {
	// Super::PostInitializeComponents();

	// UCodeCurveLib* const CurveLib = UCodeCurveLib::Instance();
	// Anim->CodeCurve.BindDynamic(CurveLib, &UCodeCurveLib::OutBack);
// }
