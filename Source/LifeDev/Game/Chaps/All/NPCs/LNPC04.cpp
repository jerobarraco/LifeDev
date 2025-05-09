// Copyright (c) 2043 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC04.h"

#include "Interact/CInteract.h"

#include "CQuickMesh.h"

// TODO make an instance class and use that instead in chap00
ALNPC04::ALNPC04():Super() {
	UseAnim = false;
	IsLocked = true;
	Texts = { NSLOCTEXT("NPC04", "State0", "...") };

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh1(TEXT("/Game/LifeDev/Game/Chars/Char22/Parts/Pelvis.Pelvis"));
	Pelvis->SetStaticMesh(CMesh1.Object);
	Pelvis->SetRelativeLocation(FVector(0,0,60));

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

	Interact->SetRelativeLocation(FVector(0,0,72.5));
	Interact->SetBoxExtent(FVector(25,20,75));

	// TODO actually move this to the child instance (since this class itself doesn't need to fade).
	UseFade = true;

	PoseBase.InteractOrg = FVector(0,0,72.5);
	PoseBase.InteractExt = FVector(25,20,75);
	PoseBase.Head.SetLocation(FVector(0,0,40));
	PoseBase.Pelvis.SetLocation(FVector(0,0,60));
	PoseBase.ArmL1.SetLocation(FVector(0,20,30));
	PoseBase.ArmR1.SetRotation(FRotator(0,180,0).Quaternion());
	PoseBase.ArmR1.SetLocation(FVector(0,-20,30));
	PoseBase.ArmL2.SetLocation(FVector(0,-2.5,-25));
	PoseBase.ArmR2.SetLocation(FVector(0,-2.5,-25));
	PoseBase.LegL1.SetLocation(FVector(0,10,-15));
	PoseBase.LegR1.SetLocation(FVector(0,-10,-15));
	PoseBase.LegL2.SetLocation(FVector(0,0,-20));
	PoseBase.LegR2.SetLocation(FVector(0,0,-20));
	PoseBase.FootL.SetLocation(FVector(0,0,-20));
	PoseBase.FootR.SetLocation(FVector(0,0,-20));
	
	PoseChair.InteractOrg = FVector(5.000000, 5.000000, 77.500000);
	PoseChair.InteractExt = FVector(30.000000, 25.000000, 75.000000);
	PoseChair.Head.SetLocation(FVector(0.000000, 0.000000, 40.000000));
	PoseChair.Head.SetRotation(FRotator(4.980925, -10.019002, -0.436880).Quaternion());
	PoseChair.Pelvis.SetLocation(FVector(5.000000, 0.000000, 60.000000));
	PoseChair.Pelvis.SetRotation(FRotator(90.000000, 0.000000, 0.000000).Quaternion());
	PoseChair.Torso.SetRotation(FRotator(-85.000000, -90.000000, 90.000000).Quaternion());
	PoseChair.ArmL2.SetLocation(FVector(0.000000, -2.500000, -25.000000));
	PoseChair.ArmL2.SetRotation(FRotator(21.999811, -37.982326, -12.276986).Quaternion());
	PoseChair.ArmL1.SetLocation(FVector(-0.000000, 20.000000, 30.000000));
	PoseChair.ArmL1.SetRotation(FRotator(27.025840, -46.051430, -20.795519).Quaternion());
	PoseChair.ArmR2.SetLocation(FVector(0.000000, -2.500000, -25.000000));
	PoseChair.ArmR2.SetRotation(FRotator(-44.789747, 11.123475, 64.555278).Quaternion());
	PoseChair.ArmR1.SetLocation(FVector(0.000000, -20.000000, 30.000000));
	PoseChair.ArmR1.SetRotation(FRotator(-61.015985, -151.987788, 4.047078).Quaternion());
	PoseChair.LegL2.SetLocation(FVector(0.000000, 0.000000, -20.000000));
	PoseChair.LegL2.SetRotation(FRotator(-50.000000, 0.000000, -0.000000).Quaternion());
	PoseChair.LegL1.SetLocation(FVector(0.000000, 10.000000, -15.000000));
	PoseChair.LegL1.SetRotation(FRotator(-49.740862, 7.750797, -5.929943).Quaternion());
	PoseChair.LegR2.SetLocation(FVector(-0.000000, 0.000000, -20.000000));
	PoseChair.LegR2.SetRotation(FRotator(-39.817297, -6.515426, 4.182729).Quaternion());
	PoseChair.LegR1.SetLocation(FVector(0.000000, -10.000000, -15.000000));
	PoseChair.LegR1.SetRotation(FRotator(-40.000000, 0.000000, 10.000000).Quaternion());
	PoseChair.FootL.SetLocation(FVector(0.000000, 0.000000, -20.000000));
	PoseChair.FootL.SetRotation(FRotator(-25.000000, 0.000000, -0.000000).Quaternion());
	PoseChair.FootR.SetLocation(FVector(0.000000, -0.000000, -20.000000));
	PoseChair.FootR.SetRotation(FRotator(10.000000, -0.000000, -0.000000).Quaternion());
}

