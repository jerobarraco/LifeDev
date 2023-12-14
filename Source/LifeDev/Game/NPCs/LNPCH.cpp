// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCH.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

ALNPCH::ALNPCH():Super() {
	UseAnim = false;
	Locked = true;
	Texts = { FText::FromString("Talk") };
	
	Pelvis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pelvis"));
	Pelvis->SetupAttachment(Mesh);

	Torso = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torso"));
	Torso->SetupAttachment(Pelvis);

	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Torso);

	ArmL1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmL1"));
	ArmL1->SetupAttachment(Torso);
		
	ArmR1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmR1"));
	ArmR1->SetupAttachment(Torso);

	ArmL2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmL2"));
	ArmL2->SetupAttachment(ArmL1);
	ArmR2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmR2"));
	ArmR2->SetupAttachment(ArmR1);
	
	LegL1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegL1"));
	LegL1->SetupAttachment(Pelvis);
	
	LegR1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegR1"));
	LegR1->SetupAttachment(Pelvis);

	LegL2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegL2"));
	LegL2->SetupAttachment(LegL1);

	LegR2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegR2"));
	LegR2->SetupAttachment(LegR1);

	FootL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootL"));
	FootL->SetupAttachment(LegL2);
	
	FootR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootR"));
	FootR->SetupAttachment(LegR2);

	/// anim fade
	AnimFade->Meshes = {
		Mesh, Head, Torso, Pelvis,
		ArmL1, ArmL2, ArmR1, ArmR2,
		LegL1, LegL2, LegR1, LegR2,
		FootL, FootR
	};
}

void ALNPCH::SetPose(const FNPCHPose& Pose) {
	Pelvis->SetRelativeTransform(Pose.Pelvis);
	Torso->SetRelativeTransform(Pose.Torso);
	Head->SetRelativeTransform(Pose.Head);
	ArmL1->SetRelativeTransform(Pose.ArmL1);
	ArmL2->SetRelativeTransform(Pose.ArmL2);
	ArmR1->SetRelativeTransform(Pose.ArmR1);
	ArmR2->SetRelativeTransform(Pose.ArmR2);
	LegL1->SetRelativeTransform(Pose.LegL1);
	LegL2->SetRelativeTransform(Pose.LegL2);
	LegR1->SetRelativeTransform(Pose.LegR1);
	LegR2->SetRelativeTransform(Pose.LegR2);
	FootL->SetRelativeTransform(Pose.FootL);
	FootR->SetRelativeTransform(Pose.FootR);

	Interact->SetRelativeLocation(Pose.InteractOrg);
	Interact->SetBoxExtent(Pose.InteractExt);
}

