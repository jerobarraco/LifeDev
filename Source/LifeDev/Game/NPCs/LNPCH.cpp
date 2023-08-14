// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCH.h"

#include "Interact/CInteract.h"

ALNPCH::ALNPCH():Super() {
	AnimEnabled = false;
	Locked = true;
	Texts = { FText::FromString("Talk") };
	
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Char18.Char18"));
	// Mesh->SetStaticMesh(CMesh.Object);
	// Mesh->SetRelativeLocation(FVector(-30.000000,30.000000,10.092946));

	// static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		// CMat(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/Ghost/Ghost_MI.Ghost_MI"));
	// FadeMatClass = CMat.Object;
	
	// (X=-30.000000,Y=30.000000,Z=-40.000000)
	Pelvis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pelvis"));
	Pelvis->SetupAttachment(Mesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh1(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Pelvis.Char18-Pelvis"));
	Pelvis->SetStaticMesh(CMesh1.Object);

	Torso = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torso"));
	Torso->SetupAttachment(Pelvis);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh2(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Torso.Char18-Torso"));
	Torso->SetStaticMesh(CMesh2.Object);
	// (X=-30.000000,Y=30.000000,Z=-40.000000)

	// (X=-30.000000,Y=30.000000,Z=-80.000000)
	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Torso);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh3(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Head.Char18-Head"));
	Head->SetStaticMesh(CMesh3.Object);

	// (X=-30.000000,Y=10.000000,Z=-75.000000)
	ArmL1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmL1"));
	ArmL1->SetupAttachment(Torso);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh4(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Arm1.Char18-Arm1"));
	ArmL1->SetStaticMesh(CMesh4.Object);
		
	ArmR1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmR1"));
	ArmR1->SetupAttachment(Torso);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh5(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Arm1.Char18-Arm1"));
	ArmR1->SetStaticMesh(CMesh5.Object);

	// (X=-30.000000,Y=12.500000,Z=-55.000000)
	ArmL2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmL2"));
	ArmL2->SetupAttachment(ArmL1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh6(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Arm2.Char18-Arm2"));
	ArmL2->SetStaticMesh(CMesh6.Object);
	ArmR2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmR2"));
	ArmR2->SetupAttachment(ArmR1);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh7(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Arm2.Char18-Arm2"));
	ArmR2->SetStaticMesh(CMesh7.Object);

	// (X=-30.000000,Y=20.000000,Z=-25.000000)
	LegL1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegL1"));
	LegL1->SetupAttachment(Pelvis);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh8(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Leg1.Char18-Leg1"));
	LegL1->SetStaticMesh(CMesh8.Object);
	
	LegR1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegR1"));
	LegR1->SetupAttachment(Pelvis);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh9(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Leg1.Char18-Leg1"));
	LegR1->SetStaticMesh(CMesh9.Object);

	// (X=-30.000000,Y=20.000000,Z=-10.000000)
	LegL2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegL2"));
	LegL2->SetupAttachment(LegL1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh10(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Leg2.Char18-Leg2"));
	LegL2->SetStaticMesh(CMesh10.Object);

	LegR2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegR2"));
	LegR2->SetupAttachment(LegR1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh11(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Leg2.Char18-Leg2"));
	LegR2->SetStaticMesh(CMesh11.Object);


	// (X=-30.000000,Y=20.000000,Z=5.000000)
	FootL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootL"));
	FootL->SetupAttachment(LegL2);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh12(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Foot.Char18-Foot"));
	FootL->SetStaticMesh(CMesh12.Object);
	
	FootR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootR"));
	FootR->SetupAttachment(LegR2);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh13(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Foot.Char18-Foot"));
	FootR->SetStaticMesh(CMesh13.Object);
	
	PoseBase.InteractOrg = FVector(32.500000,-30.000000,59.988557);
	PoseBase.InteractExt = FVector(2.500000,0.000000,70.000000);
	PoseBase.Pelvis.SetLocation(FVector(0,0,50));
	PoseBase.Head.SetLocation(FVector(0,0,40));

	PoseBase.ArmL1.SetLocation(FVector(0.000000,15.000000,35.000000));
	PoseBase.ArmL1.SetRotation(FRotator(0, 180, 0).Quaternion());
	PoseBase.ArmR1.SetLocation(FVector(0.000000,-15.000000,35.000000));
	PoseBase.ArmL2.SetLocation(FVector(0,-2.5,-20));
	PoseBase.ArmR2.SetLocation(FVector(0,-2.5,-20));
	PoseBase.LegL1.SetLocation(FVector(0.000000,-10.000000,-15.000000));
	PoseBase.LegR1.SetLocation(FVector(0.000000,10.000000,-15.000000));
	PoseBase.LegL2.SetLocation(FVector(0.000000,0.000000,-15.000000));
	PoseBase.LegR2.SetLocation(FVector(0.000000,0.000000,-15.000000));
	PoseBase.FootL.SetLocation(FVector(0.000000,0.000000,-15.000000));
	PoseBase.FootR.SetLocation(FVector(0.000000,0.000000,-15.000000));
	SetPose(PoseBase);
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

// void ALNPCH::BeginPlay() {
	// Super::BeginPlay();
	// has to be done on begin play or the bp wont save
	// also i think the translucent material breaks the outline shader
	// maybe use a different animation then, maybe a color override
	// SetFadeMat();
// }

// void ALNPCH::SetFadeMat_Implementation() {
	// Super::SetFadeMat_Implementation();
	// Pelvis->SetMaterial(0, FadeMat);
	// Torso->SetMaterial(0, FadeMat);
	// Head->SetMaterial(0, FadeMat);
	// ArmL1->SetMaterial(0, FadeMat);
	// ArmR1->SetMaterial(0, FadeMat);
	// ArmL2->SetMaterial(0, FadeMat);
	// ArmR2->SetMaterial(0, FadeMat);
	// FootL->SetMaterial(0, FadeMat);
	// FootR->SetMaterial(0, FadeMat);
	// LegL1->SetMaterial(0, FadeMat);
	// LegL2->SetMaterial(0, FadeMat);
	// LegR1->SetMaterial(0, FadeMat);
	// LegR2->SetMaterial(0, FadeMat);
// }
