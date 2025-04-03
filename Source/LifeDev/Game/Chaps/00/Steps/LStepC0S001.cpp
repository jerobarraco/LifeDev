// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S001.h"

#include "CQuickMesh.h"
#include "Camera/CameraComponent.h"
#include "Interact/Animator/CAnimatorTrans.h"

#include "LifeDev/Core/Consts/ConstItems.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC0S001::ALStepC0S001():Super() {
	Name = FName("C0S1");
	DlgId = FName("Intro");
	UseFBDlgAuto = true;
	FBDlgAutoTo = 1.05;
	InputEnabled = false;
	UseFadeTime = true;
	UseFade = true; // important since the door, also due to the load
	CamBlendTime = 0; // doesn't get along with fade
	FinishPostWait = false;
	TeleportChar = true;
	UseRain = true;
	ItemsRem = {
		LDConsts::Items::Tape0, "Bottle00", "Bottle01"
	}; // ensure you don't carry it.
	Cam->SetConstraintAspectRatio(true);
	Cam->SetAspectRatio(2);
	UseGhosts = true;
	GhostPos = FVector(210,-42,-65);

	Target = CreateDefaultSubobject<USceneComponent>(TEXT("Target"));
	Target->SetupAttachment(Root);
	
#if WITH_EDITORONLY_DATA
	TargetArrow = CreateDefaultSubobject<UCQuickMesh>(TEXT("TargetArrow"));
	TargetArrow->SetupAttachment(Target);
	TargetArrow->SetHiddenInGame(true);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CCam (TEXT("/Niagara/DefaultAssets/S_Arrow.S_Arrow"));
	if (CCam.Succeeded()) TargetArrow->SetStaticMesh(CCam.Object);
#endif

	Anim = CreateDefaultSubobject<UCAnimatorTrans>(TEXT("Anim"));
	Anim->TRoot = Cam;
}

void ALStepC0S001::Start_Implementation() {
	Super::Start_Implementation();
	Anim->TEnd = Target->GetRelativeTransform();
	FB->SetMax(1);
	FB->OnChange.AddUniqueDynamic(this, &ALStepC0S001::FBUpd);
	// FB->SetVal(1, 10);
}

void ALStepC0S001::Stop_Implementation() {
	FB->OnChange.RemoveAll(this);
	Super::Stop_Implementation();
}

void ALStepC0S001::FBUpd(const float Value) {
	// notice this depends on the tick interval for the FB. it will give the most accurate animation though.
	// i will go with something simple for now.
	Anim->Update(Value); // set the value to match the fb. let the anim do the calculations.
}

// don't change the fb val for the next chapter here. since it's not the responsibility of this class.
// in other words: don't put your nose where it doesn't belong. don't do what you're not asked.
// because you might not know what is needed (it was actually introducing a soft error by using the default time.)
// also it makes it harder to find the origin.



// APlayerController* const Controller = World->GetFirstPlayerController();
// if (UNLIKELY(!Controller)) return;
// if (CamTarget == this && LIKELY(IsValid(Cam))) Cam->SetComponentTickEnabled(true);

// Controller->SetViewTargetWithBlend(CamTarget, CamBlendTime, VTBlend_Cubic);