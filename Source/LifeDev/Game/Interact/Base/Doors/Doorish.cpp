// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "Doorish.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Interact/Animator/CAnimatorFade.h"

ADoorish::ADoorish():Super() {
	UseFade = false;
	AnimFade->Meshes.Empty(); // avoid getting the material scrambled
	UseRewardDestroy = false; // will also avoid scrambled material
	UseAutoActivate = true;
	UseFBAnimFPS = true;

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
