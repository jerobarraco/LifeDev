// Copyright Jerónimo Barraco-Mármol

#include "PianoLid.h"

#include "CQuickMesh.h"
#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

APianoLid::APianoLid():Super() {
	Texts = { NSLOCTEXT("PianoLid", "State0", "") };
	StateNum = 2;
	UseAnim = true;
	UseFade = false;
	DisableWhileAnim = false; // avoid re-enabling the Interact once triggered
	// UseAutoActivate = false; // not manually triggerable // redundant

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Music/Piano/Piano_Lid.Piano_Lid"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-72.5,25,-70));
	Mesh->SetCastAllShadows(true);

	Interact->SetRelativeLocation(FVector(72.5,-25,80));
	Interact->SetBoxExtent(FVector(57.2,5,10));
	SFX->SetRelativeLocation(FVector(72.5,-25,80));

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX (TEXT("/Game/LifeDev/Game/Inters/Generic/Drawers/Nightstand_Modern_Drawers_Wood_Close_x6_Fienup_001-04.Nightstand_Modern_Drawers_Wood_Close_x6_Fienup_001-04"));
	SFX_Trigger = CSFX.Object;

	Anim->Curve = nullptr;
	Anim->Duration = 1;
	Anim->IsAdditive = true;
	Anim->TEnd.SetRotation(FRotator(0,0,90).Quaternion());
}

void APianoLid::PostInitializeComponents() {
	Super::PostInitializeComponents();

	UCodeCurveLib* const CurveLib = UCodeCurveLib::Instance();
	Anim->CodeCurve.BindDynamic(CurveLib, &UCodeCurveLib::InExpo);
}
