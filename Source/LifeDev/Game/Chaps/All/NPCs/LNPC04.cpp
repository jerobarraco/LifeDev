// Copyright (c) 2043 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC04.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"

ALNPC04::ALNPC04():Super() {
	UseAnim = false;
	Locked = true;
	Texts = { FText::FromString("...") };

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh1(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/Pelvis.Pelvis"));
	Pelvis->SetStaticMesh(CMesh1.Object);
	Pelvis->SetRelativeLocation(FVector(0,0,45));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh2(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/Torso.Torso"));
	Torso->SetStaticMesh(CMesh2.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh3(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/Head.Head"));
	Head->SetStaticMesh(CMesh3.Object);
	Head->SetRelativeLocation(FVector(0,0,40));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh4(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/ArmL1.ArmL1"));
	ArmL1->SetStaticMesh(CMesh4.Object);
	ArmL1->SetRelativeLocation(FVector(0,20,30));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh5(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/ArmL1.ArmL1"));
	ArmR1->SetStaticMesh(CMesh5.Object);
	ArmR1->SetRelativeRotation(FRotator(0,180,0).Quaternion());
	ArmR1->SetRelativeLocation(FVector(0,-20,30));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh6(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/ArmL2.ArmL2"));
	ArmL2->SetStaticMesh(CMesh6.Object);
	ArmL2->SetRelativeLocation(FVector(0,-2.5,-25));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh7(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/ArmL2.ArmL2"));
	ArmR2->SetStaticMesh(CMesh7.Object);
	ArmR2->SetRelativeLocation(FVector(0,-2.5,-25));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh8(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/LegL1.LegL1"));
	LegL1->SetStaticMesh(CMesh8.Object);
	LegL1->SetRelativeLocation(FVector(0,10,-15));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh9(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/LegL1.LegL1"));
	LegR1->SetStaticMesh(CMesh9.Object);
	LegR1->SetRelativeLocation(FVector(0,-10,-15));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh10(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/LegL2.LegL2"));
	LegL2->SetStaticMesh(CMesh10.Object);
	LegL2->SetRelativeLocation(FVector(0.000000,0,-20));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh11(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/LegL2.LegL2"));
	LegR2->SetStaticMesh(CMesh11.Object);
	LegR2->SetRelativeLocation(FVector(0,0,-20));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh12(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/FootL.FootL"));
	FootL->SetStaticMesh(CMesh12.Object);
	FootL->SetRelativeLocation(FVector(0,0,-20));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh13(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/FootL.FootL"));
	FootR->SetStaticMesh(CMesh13.Object);
	FootR->SetRelativeLocation(FVector(0,0,-20));

	// TODO adjust interact 
	Interact->SetRelativeLocation(FVector(0,0,65));
	Interact->SetBoxExtent(FVector(25,20,65));

	AnimFade->SetNewMat();

	// TODo remove pose scold
	// TODO set appropriate poses
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
}

