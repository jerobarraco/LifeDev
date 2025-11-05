// Copyright Jerónimo Barraco-Mármol

#include "Pot.h"

#include "Components/AudioComponent.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"

#include "JUtils/Actors/CQuickMesh.h"

APot::APot():Super() {
	Texts = {
		NSLOCTEXT("Pot", "State0", "Pot Full"), // closed
		NSLOCTEXT("Pot", "State1", "Pot")
	};
	RewardFlash = 0;
	UseRewardDestroy = false;
	StateNum = 2;
	UseAnim = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Pot/Pot01_Base"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));
	Mesh->SetUseDynShadow(true);

	Lid = CreateDefaultSubobject<UCQuickMesh>(TEXT("Lid"));
	Lid->SetupAttachment(Mesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMeshLid(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Pot/Pot01_Lid"));
	Lid->SetStaticMesh(CMeshLid.Object);
	Lid->SetRelativeLocation(FVector(0,0,0));
	Lid->SetUseDynShadow(true);

	AnimFade->Meshes.AddUnique(Lid);
	Anim->TRoot = Lid;
	Anim->TEnd.SetRotation(FRotator(0, -10, 0).Quaternion());

	Interact->SetRelativeLocation(FVector(27.5,-17.5,12.5));
	Interact->SetBoxExtent(FVector(17.5,12.5,12.5));
	SFX->SetRelativeLocation(FVector(27.5,-17.5,12.5));
	
	// stolen from paper. maybe get a new one?
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndClose(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Crush_Rattle_Metal_Scrap_Debris_UC_04-02.Crush_Rattle_Metal_Scrap_Debris_UC_04-02"));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSndOpen(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Trashcan/Crush_Rattle_Metal_Scrap_Debris_UC_04-03.Crush_Rattle_Metal_Scrap_Debris_UC_04-03"));
}

