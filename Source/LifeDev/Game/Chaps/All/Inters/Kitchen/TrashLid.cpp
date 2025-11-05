// Copyright Jerónimo Barraco-Mármol

#include "TrashLid.h"

#include "Components/AudioComponent.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ATrashLid::ATrashLid():Super() {
	UseAutoActivate = true; // usually, it is auto active. like a door.
	UseRewardDestroy = false;
	RewardFlash = 0;
	StateNum = 2;
	Texts = {
		NSLOCTEXT("TrashLid", "State0", "Open"),
		NSLOCTEXT("TrashLid", "State1", "Close")};

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Lid"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));
	Mesh->SetUseDynShadow(true);

	Interact->SetRelativeLocation(FVector(-20,0,2.5));
	Interact->SetBoxExtent(FVector(20,20,2.5));
	SFX->SetRelativeLocation(FVector(0,0,0));
	
	// stolen from paper. maybe get a new one?
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndClose(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Crush_Rattle_Metal_Scrap_Debris_UC_04-02"));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndOpen(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Crush_Rattle_Metal_Scrap_Debris_UC_04-03"));

	SFXs = { CSndClose.Object, CSndOpen.Object };
	SFX->SetRelativeLocation(FVector(-22.5,-0.5,12.5));

	UseAnim = true;
	Anim->Duration = .6;
	Anim->TEnd.SetRotation(FRotator(-43.2,0,0).Quaternion());
}

