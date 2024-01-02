// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "Doorish.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Interact/Animator/CAnimatorFade.h"

ADoorish::ADoorish():Super() {
	UseAnim = true;
	Anim->IsAdditive = true;
	// visual: run at 60 fps since it's movement
	Anim->SetComponentTickInterval(1/60.f);
	
	AnimFade->Meshes.Empty(); // avoid getting the material scrambled
	UseRewardFade = false; // will also avoid scrambled material

	Mesh->SetCastAllShadows(true);
	// important to call Super here. since this is called before the child constructor
	// if the child overrides setmobility it could lead to a crash
	Super::SetMobility(EComponentMobility::Movable);
	
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SOpen(TEXT("/Game/LifeDev/Game/Inters/Doors/Door00/SBvfe1_Door_Handle_D_002.SBvfe1_Door_Handle_D_002"));
	SFX_Start = {nullptr, SOpen.Object};
	// SFX_Open = SOpen.Object;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SClose(TEXT("/Game/LifeDev/Game/Inters/Doors/Door00/door-02.door-02"));
	// SFX_CloseEnd = SClose.Object;
	SFX_Stop = {SClose.Object, nullptr};
	static ConstructorHelpers::FObjectFinder<USoundBase>
		SLocked(TEXT("/Game/LifeDev/Game/Inters/Doors/Door00/door_lock_007.door_lock_007"));
	SFX_Locked = SLocked.Object;
}
