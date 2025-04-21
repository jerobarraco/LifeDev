// Copyright (c) 2033 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC03.h"

#include "Interact/CInteract.h"

#include "CQuickMesh.h"

ALNPC03::ALNPC03():Super() {
	UseAnim = false;
	IsLocked = true;
	Texts = { NSLOCTEXT("NPC03", "State0", "...") };

	// (X=-30.000000,Y=30.000000,Z=-40.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh1(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/Hip.Hip"));
	Pelvis->SetStaticMesh(CMesh1.Object);
	Pelvis->SetRelativeLocation(FVector(0,0,45));

	// (X=-30.000000,Y=30.000000,Z=-40.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh2(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/Torso.Torso"));
	Torso->SetStaticMesh(CMesh2.Object);

	// (X=-30.000000,Y=30.000000,Z=-80.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh3(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/HeadHair.HeadHair"));
	Head->SetStaticMesh(CMesh3.Object);
	Head->SetRelativeLocation(FVector(0,0,35));

	// (X=-30.000000,Y=10.000000,Z=-75.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh4(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/LeftArm_High.LeftArm_High"));
	ArmL1->SetStaticMesh(CMesh4.Object);
	ArmL1->SetRelativeLocation(FVector(0,15,30));
	// ArmL1->SetRelativeRotation(FRotator(0, 180, 0));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh5(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/Right_Arm_High.Right_Arm_High"));
	ArmR1->SetStaticMesh(CMesh5.Object);
	ArmR1->SetRelativeLocation(FVector(0,-15,30));

	// (X=-30.000000,Y=12.500000,Z=-55.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh6(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/LeftArm_Low.LeftArm_Low"));
	ArmL2->SetStaticMesh(CMesh6.Object);
	ArmL2->SetRelativeLocation(FVector(0,2.5,-20));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh7(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/Right_Arm_Low.Right_Arm_Low"));
	ArmR2->SetStaticMesh(CMesh7.Object);
	ArmR2->SetRelativeLocation(FVector(0,-2.5,-20));

	// (X=-30.000000,Y=20.000000,Z=-25.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh8(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/Left_leg_high.Left_leg_high"));
	LegL1->SetStaticMesh(CMesh8.Object);
	LegL1->SetRelativeLocation(FVector(0,10,-10.000000));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh9(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/right_leg_high.right_leg_high"));
	LegR1->SetStaticMesh(CMesh9.Object);
	LegR1->SetRelativeLocation(FVector(0,-10,-10));

	// (X=-30.000000,Y=20.000000,Z=-10.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh10(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/Left_leg_low.Left_leg_low"));
	LegL2->SetStaticMesh(CMesh10.Object);
	LegL2->SetRelativeLocation(FVector(0.000000,0,-15));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh11(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/right_leg_low.right_leg_low"));
	LegR2->SetStaticMesh(CMesh11.Object);
	LegR2->SetRelativeLocation(FVector(0,0,-15));

	// (X=-30.000000,Y=20.000000,Z=5.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh12(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/Left_foot.Left_foot"));
	FootL->SetStaticMesh(CMesh12.Object);
	FootL->SetRelativeLocation(FVector(0,0,-15));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh13(TEXT("/Game/LifeDev/Game/Chars/Char11/Parts/Right_foot.Right_foot"));
	FootR->SetStaticMesh(CMesh13.Object);
	FootR->SetRelativeLocation(FVector(0,0,-15));

	Interact->SetRelativeLocation(FVector(0,0,65));
	Interact->SetBoxExtent(FVector(25,20,65));
	
	PoseBase.InteractExt = FVector(25,25,55);
	PoseBase.InteractOrg = FVector(0,-5,50);
	PoseBase.Head.SetLocation(FVector(0,0,25));
	PoseBase.Pelvis.SetLocation(FVector(0,0,35));
	PoseBase.ArmL1.SetLocation(FVector(-10,0,25));
	PoseBase.ArmR1.SetLocation(FVector(10,0,25));
	PoseBase.ArmL2.SetLocation(FVector(-2.5,0,-15));
	PoseBase.ArmR2.SetLocation(FVector(2.5,0,-15));
	PoseBase.LegL1.SetLocation(FVector(-5,0,-10));
	PoseBase.LegR1.SetLocation(FVector(5,0,-10));
	PoseBase.LegL2.SetLocation(FVector(0,0,-10));
	PoseBase.LegR2.SetLocation(FVector(0,0,-10));
	PoseBase.FootL.SetLocation(FVector(0,0,-10));
	PoseBase.FootR.SetLocation(FVector(0,0,-10));
	
	PoseBase.Pelvis.SetLocation(FVector(0,0,45));
	PoseBase.Head.SetLocation(FVector(0,0,35));
	PoseBase.ArmL1.SetLocation(FVector(0,15,30));
	PoseBase.ArmR1.SetLocation(FVector(0,-15,30));
	PoseBase.ArmL2.SetLocation(FVector(0,2.5,-20));
	PoseBase.ArmR2.SetLocation(FVector(0,-2.5,-20));
	PoseBase.LegL1.SetLocation(FVector(0,10,-10.000000));
	PoseBase.LegR1.SetLocation(FVector(0,-10,-10));
	PoseBase.LegL2.SetLocation(FVector(0.000000,0,-15));
	PoseBase.LegR2.SetLocation(FVector(0,0,-15));
	PoseBase.FootL.SetLocation(FVector(0,0,-15));
	PoseBase.FootR.SetLocation(FVector(0,0,-15));
	PoseBase.InteractOrg = FVector(0,0,65);
	PoseBase.InteractExt = FVector(25,20,65);
	
	PoseScold = PoseBase;
	PoseScold.InteractExt = FVector(25,25,55);
	PoseScold.InteractOrg = FVector(0,-5,50);
	
	PoseScold.Head.SetLocation(FVector(0.000000, 0.000000, 35.000000));
	PoseScold.Head.SetRotation(FRotator(0.000000, -5.000000, 0.000000).Quaternion());
	PoseScold.Pelvis.SetLocation(FVector(-0.435779, 0.000000, 44.980973));
	PoseScold.Pelvis.SetRotation(FRotator(5.000000, 0.000000, -0.000000).Quaternion());
	PoseScold.Torso.SetRotation(FRotator(-10.000000, 0.000000, -0.000000).Quaternion());
	PoseScold.ArmL2.SetLocation(FVector(-0.000000, 2.500000, -20.000000));
	PoseScold.ArmL2.SetRotation(FRotator(35.000000, 0.000000, -0.000000).Quaternion());
	PoseScold.ArmL1.SetLocation(FVector(0.000000, 15.000000, 30.000000));
	PoseScold.ArmL1.SetRotation(FRotator(25.000000, 0.000000, -0.000000).Quaternion());
	PoseScold.ArmR2.SetLocation(FVector(0.000000, -2.500000, -20.000000));
	PoseScold.ArmR2.SetRotation(FRotator(0.000000, 0.000000, -25.000000).Quaternion());
	PoseScold.ArmR1.SetLocation(FVector(0.000000, -15.000000, 30.000000));
	PoseScold.ArmR1.SetRotation(FRotator(0.000000, -0.000000, 15.000000).Quaternion());
	PoseScold.LegL2.SetLocation(FVector(0.000000, 0.000000, -15.000000));
	PoseScold.LegL2.SetRotation(FRotator(-5.000000, 0.000000, -0.000000).Quaternion());
	PoseScold.LegL1.SetLocation(FVector(0.000000, 10.000000, -10.000000));
	PoseScold.LegL1.SetRotation(FRotator(0.000000, 0.000000, -0.000000).Quaternion());
	PoseScold.LegR2.SetLocation(FVector(0.000000, 0.000000, -15.000000));
	PoseScold.LegR2.SetRotation(FRotator(-5.000000, 0.000000, -0.000000).Quaternion());
	PoseScold.LegR1.SetLocation(FVector(0.000000, -10.000000, -10.000000));
	PoseScold.LegR1.SetRotation(FRotator(0.000000, 0.000000, -0.000000).Quaternion());
	PoseScold.FootL.SetLocation(FVector(-0.000000, 0.000000, -15.000000));
	PoseScold.FootR.SetLocation(FVector(-0.000000, 0.000000, -15.000000));


	PoseChair.InteractOrg = FVector(5.000000, 0.000000, 70.000000);
	PoseChair.InteractExt = FVector(30.000000, 25.000000, 65.000000);
	PoseChair.Head.SetLocation(FVector(-0.000000, -0.000000, 35.000000));
	PoseChair.Head.SetRotation(FRotator(0.000000, 5.000000, -0.000000).Quaternion());
	PoseChair.Pelvis.SetLocation(FVector(0.000000, 0.000000, 45.000000));
	PoseChair.Pelvis.SetRotation(FRotator(85.000000, -0.000000, -0.000000).Quaternion());
	PoseChair.Torso.SetRotation(FRotator(-90.000000, -179.999923, 179.999923).Quaternion());
	PoseChair.ArmL2.SetLocation(FVector(0.000000, 2.500000, -20.000000));
	PoseChair.ArmL2.SetRotation(FRotator(4.923850, -0.870385, 79.962576).Quaternion());
	PoseChair.ArmL1.SetLocation(FVector(0.000000, 15.000000, 30.000000));
	PoseChair.ArmL1.SetRotation(FRotator(43.079517, -20.753571, -14.510818).Quaternion());
	PoseChair.ArmR2.SetLocation(FVector(-0.000000, -2.500000, -20.000000));
	PoseChair.ArmR2.SetRotation(FRotator(19.290997, 5.381521, -74.084734).Quaternion());
	PoseChair.ArmR1.SetLocation(FVector(0.000000, -15.000000, 30.000000));
	PoseChair.ArmR1.SetRotation(FRotator(38.380802, 19.278960, 12.252937).Quaternion());
	PoseChair.LegL2.SetLocation(FVector(-0.000000, -0.000000, -15.000000));
	PoseChair.LegL2.SetRotation(FRotator(-60.000000, -0.000000, -0.000000).Quaternion());
	PoseChair.LegL1.SetLocation(FVector(0.000000, 10.000000, -10.000000));
	PoseChair.LegL1.SetRotation(FRotator(-5.000000, -0.000000, 0.000000).Quaternion());
	PoseChair.LegR2.SetLocation(FVector(0.000000, 0.000000, -15.000000));
	PoseChair.LegR2.SetRotation(FRotator(-65.000000, 0.000000, -0.000000).Quaternion());
	PoseChair.LegR1.SetLocation(FVector(-0.000000, -10.000000, -10.000000));
	PoseChair.LegR1.SetRotation(FRotator(-5.000000, -0.000000, 0.000000).Quaternion());
	PoseChair.FootL.SetLocation(FVector(0.000000, 0.000000, -15.000000));
	PoseChair.FootL.SetRotation(FRotator(-35.000000, 0.000000, -0.000000).Quaternion());
	PoseChair.FootR.SetLocation(FVector(0.000000, -0.000000, -15.000000));
	PoseChair.FootR.SetRotation(FRotator(-25.000000, -0.000000, -0.000000).Quaternion());
}

