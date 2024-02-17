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
		CSFX (TEXT("/Game/LifeDev/Game/Inters/Doors/Door00/door-02.door-02"));
	SFX_Stop = {nullptr, CSFX.Object}; // TODO test, otherwise use SFX_Trigger
	SFX_Trigger = nullptr;

	Anim->Curve = nullptr;
	Anim->Duration = 1;
	Anim->IsAdditive = true;
	Anim->TEnd.SetRotation(FRotator(0,0,90).Quaternion());
}

void APianoLid::PostInitializeComponents() {
	Super::PostInitializeComponents();
	// return;
	UCodeCurveLib* const CurveLib = UCodeCurveLib::Instance();
	Anim->CodeCurve.BindDynamic(CurveLib, &UCodeCurveLib::InExpo);
}
