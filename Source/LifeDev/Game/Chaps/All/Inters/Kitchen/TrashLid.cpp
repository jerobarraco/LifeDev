// Copyright Jerónimo Barraco-Mármol

#include "TrashLid.h"

#include "Components/AudioComponent.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JUtils/Actors/CQuickMesh.h"

ATrashLid::ATrashLid():Super() {
	Texts = { FText::FromString(TEXT("Open")), FText::FromString(TEXT("Close")) };
	RewardFlash = 0;
	UseRewardFade = false;
	StateNum = 2;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Lid.Lid"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));
	
	Interact->SetRelativeLocation(FVector(-20,0,2.5));
	Interact->SetBoxExtent(FVector(20,20,2.5));
	SFX->SetRelativeLocation(FVector(0,0,0));
	
	// stolen from paper. maybe get a new one?
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndClose(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Crush_Rattle_Metal_Scrap_Debris_UC_04-02.Crush_Rattle_Metal_Scrap_Debris_UC_04-02"));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndOpen(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Crush_Rattle_Metal_Scrap_Debris_UC_04-03.Crush_Rattle_Metal_Scrap_Debris_UC_04-03"));

	SFX_Start = { CSndClose.Object, CSndOpen.Object };
	// SFX_Trigger = CSnd.Object;

	UseAnim = true;
	Anim->Duration = .6;
	Anim->TEnd.SetRotation(FRotator(-43.2,0,0).Quaternion());
}

