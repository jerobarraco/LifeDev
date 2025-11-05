// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Ppl.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"

APpl::APpl():Super() {
	Texts = {
		NSLOCTEXT("Ppl", "State0", "..."), // closed // TODO
	};
	RewardFlash = 0;
	UseRewardDestroy = false;
	StateNum = 1;
	UseAnim = false;
	UseFade = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Env/Ppl/Body"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetUseDynShadow(true);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CBodyMat(TEXT("/Game/LifeDev/Game/Env/Ppl/Ppl_MI"));
	Mesh->SetMaterial(0, CBodyMat.Object);
	Mesh->SetRelativeLocation(FVector(40,-40,0));
	
	Head = CreateDefaultSubobject<UCQuickMesh>(TEXT("Head"));
	Head->SetupAttachment(Mesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CHeadMesh(TEXT("/Game/LifeDev/Game/Env/Ppl/Head"));
	Head->SetStaticMesh(CHeadMesh.Object);
	Head->SetUseDynShadow(true);
	Head->SetMaterial(0, CBodyMat.Object);
	Head->SetRelativeLocation(FVector(0,0,0));
	AnimFade->Meshes.AddUnique(Head);
	AnimFade->MatBase = nullptr; // the mesh uses a palette. force using the mesh's material.

	Interact->SetRelativeLocation(FVector(-40,40,110));
	Interact->SetBoxExtent(FVector(40,25,110));
}
