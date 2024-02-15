// Copyright Jerónimo Barraco-Mármol

#include "PianoLid.h"

#include "CQuickMesh.h"
#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

APianoLid::APianoLid():Super() {
	Texts = {FText::FromString("")};
	StateNum = 2;
	UseAnim = true;
	DisableWhileAnim = false; // avoid re-enabling the interact once triggered
	SetEnabled(false); // not manually triggerable

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Piano_Lid.Piano_Lid"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-72.5,25,-70));
	
	Interact->SetRelativeLocation(FVector(72.5,-25,80));
	Interact->SetBoxExtent(FVector(57.2,5,10));
	SFX->SetRelativeLocation(FVector(72.5,-25,80));

	// TODO change on sfx days
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX (TEXT("/Game/LifeDev/Game/Inters/Music/Lid19.Lid19"));
	SFX_Stop = {nullptr, CSFX.Object}; // TODO test, otherwise use SFX_Trigger
	SFX_Trigger = CSFX.Object;
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/JUtils/Curves/PulseOut.PulseOut"));
	// niagara curves are not loaded somehow
	// Anim->Curve = nullptr;
	// Anim->Curve = CCurve.Object;
	Anim->Duration = 1;
	// this will avoid anim being set to reverse.
	// Trans.Add(FTransform(FRotator(0,0,10).Quaternion(), FVector::ZeroVector, FVector::Zero()));

	// the animation needs to play in a different way than what i want, hence i need to define tstart instead
	// Anim->IsAdditive = false;
	// Anim->TEnd = FTransform (FQuat::Identity, FVector::ZeroVector, FVector::OneVector); 
	// Anim->TStart = Anim->TEnd;
	Anim->IsAdditive = true;
	Anim->TEnd.SetRotation(FRotator(0,0,90).Quaternion());
}

// void APianoLid::SetState_Implementation(int32 NewState) {
	// overriden by AnimPlay and SetState
	// when using Trans, i need to reset since bouncing will leave it on true
	// Anim->IsReversed = false;
	// Anim->IsBouncing = true;
	// Super::SetState_Implementation(NewState);
// }

void APianoLid::PostInitializeComponents() {
	Super::PostInitializeComponents();

	UCodeCurveLib* const CurveLib = UCodeCurveLib::Instance();
	Anim->CodeCurve.BindDynamic(CurveLib, &UCodeCurveLib::InExpo);
}
