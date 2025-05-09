// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC02.h"

#include "Interact/CInteract.h"

#include "CQuickMesh.h"

ALNPC02::ALNPC02():Super() {
	UseAnim = false;
	IsLocked = true;
	Texts = { NSLOCTEXT("NPC02", "State0", "...") };

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh1(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/Pelvis.Pelvis"));
	Pelvis->SetStaticMesh(CMesh1.Object);
	Pelvis->SetRelativeLocation(FVector(0,0,35));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh2(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/Torso.Torso"));
	Torso->SetStaticMesh(CMesh2.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh3(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/Head.Head"));
	Head->SetStaticMesh(CMesh3.Object);
	Head->SetRelativeLocation(FVector(0,0,25));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh4(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/ArmL1.ArmL1"));
	ArmL1->SetStaticMesh(CMesh4.Object);
	ArmL1->SetRelativeLocation(FVector(-10,0,25));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh5(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/ArmR1.ArmR1"));
	ArmR1->SetStaticMesh(CMesh5.Object);
	ArmR1->SetRelativeLocation(FVector(10,0,25));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh6(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/ArmL2.ArmL2"));
	ArmL2->SetStaticMesh(CMesh6.Object);
	ArmL2->SetRelativeLocation(FVector(-2.5,0,-15));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh7(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/ArmR2.ArmR2"));
	ArmR2->SetStaticMesh(CMesh7.Object);
	ArmR2->SetRelativeLocation(FVector(2.5,0,-15));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh8(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/LegL1.LegL1"));
	LegL1->SetStaticMesh(CMesh8.Object);
	LegL1->SetRelativeLocation(FVector(-5,0,-10));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh9(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/LegR1.LegR1"));
	LegR1->SetStaticMesh(CMesh9.Object);
	LegR1->SetRelativeLocation(FVector(5,0,-10));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh10(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/LegL2.LegL2"));
	LegL2->SetStaticMesh(CMesh10.Object);
	LegL2->SetRelativeLocation(FVector(0,0,-10));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh11(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/LegR2.LegR2"));
	LegR2->SetStaticMesh(CMesh11.Object);
	LegR2->SetRelativeLocation(FVector(0,0,-10));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh12(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/FootL.FootL"));
	FootL->SetStaticMesh(CMesh12.Object);
	FootL->SetRelativeLocation(FVector(0,0,-10));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh13(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/FootR.FootR"));
	FootR->SetStaticMesh(CMesh13.Object);
	FootR->SetRelativeLocation(FVector(0,0,-10));

	Interact->SetRelativeLocation(FVector(0,-5,50));
	Interact->SetBoxExtent(FVector(25,25,55));

	UseFade = true;

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

	PoseChair.InteractOrg = FVector(0.000000, 5.000000, 50.000000);
	PoseChair.InteractExt = FVector(25.000000, 25.000000, 55.000000);
	PoseChair.Head.SetLocation(FVector(-0.000000, -0.000000, 25.000000));
	PoseChair.Head.SetRotation(FRotator(0.867172, -9.962711, 4.924415).Quaternion());
	PoseChair.Pelvis.SetLocation(FVector(0.000000, 0.000000, 35.000000));
	PoseChair.Pelvis.SetRotation(FRotator(0.000000, 0.000000, -79.999999).Quaternion());
	PoseChair.Torso.SetRotation(FRotator(0.000000, 0.000000, 89.999999).Quaternion());
	PoseChair.ArmL2.SetLocation(FVector(-2.500000, 0.000000, -15.000000));
	PoseChair.ArmL2.SetRotation(FRotator(-3.533287, 3.540025, -135.109220).Quaternion());
	PoseChair.ArmL1.SetLocation(FVector(-10.000000, -0.000000, 25.000000));
	PoseChair.ArmL1.SetRotation(FRotator(0.000000, -0.000000, -45.000000).Quaternion());
	PoseChair.ArmR2.SetLocation(FVector(2.500000, 0.000000, -15.000000));
	PoseChair.ArmR2.SetRotation(FRotator(-4.530509, 2.117507, -115.083770).Quaternion());
	PoseChair.ArmR1.SetLocation(FVector(10.000000, 0.000000, 25.000000));
	PoseChair.ArmR1.SetRotation(FRotator(0.000000, -0.000000, -45.000000).Quaternion());
	PoseChair.LegL2.SetLocation(FVector(-0.000000, 0.000000, -10.000000));
	PoseChair.LegL2.SetRotation(FRotator(-0.000000, -0.000000, 40.000000).Quaternion());
	PoseChair.LegL1.SetLocation(FVector(-5.000000, 0.000000, -10.000000));
	PoseChair.LegL1.SetRotation(FRotator(0.000000, 0.000000, 30.000000).Quaternion());
	PoseChair.LegR2.SetLocation(FVector(-0.000000, -0.000000, -10.000000));
	PoseChair.LegR2.SetRotation(FRotator(-0.000000, -0.000000, 45.000000).Quaternion());
	PoseChair.LegR1.SetLocation(FVector(5.000000, 0.000000, -10.000000));
	PoseChair.LegR1.SetRotation(FRotator(-0.000000, -0.000000, 30.000000).Quaternion());
	PoseChair.FootL.SetLocation(FVector(-0.000000, -0.000000, -10.000000));
	PoseChair.FootL.SetRotation(FRotator(-0.000000, -0.000000, 20.000000).Quaternion());
	PoseChair.FootR.SetLocation(FVector(-0.000000, -0.000000, -10.000000));
	PoseChair.FootR.SetRotation(FRotator(-0.000000, -0.000000, 20.000000).Quaternion());
}
