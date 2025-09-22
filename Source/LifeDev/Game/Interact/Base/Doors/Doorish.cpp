// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "Doorish.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ADoorish::ADoorish():Super() {
	UseFade = false;
	AnimFade->Meshes.Empty(); // avoid getting the material scrambled
	UseRewardDestroy = false; // will also avoid scrambled material
	UseAutoActivate = true;

	UseAnim = true;
	Anim->IsAdditive = true;

	Mesh->SetCastAllShadows(true);
	// important to call Super here. since this is called before the child constructor
	// if the child overrides SetMobility it could lead to a crash
	Super::SetMobility(EComponentMobility::Movable);

	static ConstructorHelpers::FObjectFinder<USoundBase>
		SOpen(TEXT("/Game/LifeDev/Game/Inters/Doors/Door00/SBvfe1_Door_Handle_D_002.SBvfe1_Door_Handle_D_002"));
	SFXs = {nullptr, SOpen.Object};
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SClose(TEXT("/Game/LifeDev/Game/Inters/Doors/Door00/door-02.door-02"));
	SFX_Stop = {SClose.Object, nullptr};
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SLocked(TEXT("/Game/LifeDev/Game/Inters/Doors/Door00/door_lock_007.door_lock_007"));
	SFXLocked = SLocked.Object;
}

void ADoorish::FBUpd(const float Value) {
	Anim->SetComponentTickInterval(FMath::LerpStable(1/60., 1/5., Value));
}

void ADoorish::BeginPlay() {
	Super::BeginPlay();
	// this is just an experiment
	// it lacks unbinding on feature change. also the max and min intervals should be a variable
	// that can be changed on the config.
	// and possibly this could be moved to linteract
	// and also would require a flag to disable on specific interacts,
	// e.g. the ones that use the significance (e.g. lights)
	const bool FBAnim = ULSettings::GetFeatS(this, EFeat::G_FB_ANIM);
	if (!FBAnim) return;

	
	UFlashback* const FB = UFlashback::Instance(this);
	if (UNLIKELY(!FB)) return;
	FB->OnChange.AddUniqueDynamic(this, &ADoorish::FBUpd);
}
