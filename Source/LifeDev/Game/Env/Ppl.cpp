// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Ppl.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorFade.h"

APpl::APpl():Super() {
	Texts = {
		NSLOCTEXT("Ppl", "State0", "..."), // closed
	};
	RewardFlash = 0;
	UseRewardDestroy = false;
	StateNum = 1;
	UseAnim = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Env/Ppl/Body"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetCastAllShadows(true);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CBodyMat(TEXT("/Game/LifeDev/Game/Env/Ppl/Ppl_MI"));
	Mesh->SetMaterial(0, CBodyMat.Object);
	// Mesh->SetRelativeLocation(FVector(0,0,0));
	// TODO positions
	
	Head = CreateDefaultSubobject<UCQuickMesh>(TEXT("Lid"));
	Head->SetupAttachment(Mesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CHeadMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Pot/Pot01_Lid"));
	Head->SetStaticMesh(CHeadMesh.Object);
	Head->SetCastAllShadows(true);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CHeadMat(TEXT("/ame/LifeDev/Game/Env/Ppl/Ppl_MI"));
	Head->SetMaterial(0, CHeadMat.Object);

	Head->SetRelativeLocation(FVector(0.000000,-2.747865,0.895474));

	AnimFade->Meshes.AddUnique(Head);
}
