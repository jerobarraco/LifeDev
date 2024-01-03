// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC02.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"

ALNPC02::ALNPC02():Super() {
	UseAnim = false;
	Locked = true;
	Texts = { FText::FromString("...") };

	// (X=-30.000000,Y=30.000000,Z=-40.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh1(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/Hip.Hip"));
	Pelvis->SetStaticMesh(CMesh1.Object);
	Pelvis->SetRelativeLocation(FVector(0,0,45));

	// (X=-30.000000,Y=30.000000,Z=-40.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh2(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/Torso.Torso"));
	Torso->SetStaticMesh(CMesh2.Object);

	// (X=-30.000000,Y=30.000000,Z=-80.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh3(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/HeadHair.HeadHair"));
	Head->SetStaticMesh(CMesh3.Object);
	Head->SetRelativeLocation(FVector(0,0,35));

	// (X=-30.000000,Y=10.000000,Z=-75.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh4(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/LeftArm_High.LeftArm_High"));
	ArmL1->SetStaticMesh(CMesh4.Object);
	ArmL1->SetRelativeLocation(FVector(0,15,30));
	// ArmL1->SetRelativeRotation(FRotator(0, 180, 0));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh5(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/Right_Arm_High.Right_Arm_High"));
	ArmR1->SetStaticMesh(CMesh5.Object);
	ArmR1->SetRelativeLocation(FVector(0,-15,30));

	// (X=-30.000000,Y=12.500000,Z=-55.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh6(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/LeftArm_Low.LeftArm_Low"));
	ArmL2->SetStaticMesh(CMesh6.Object);
	ArmL2->SetRelativeLocation(FVector(0,2.5,-20));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh7(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/Right_Arm_Low.Right_Arm_Low"));
	ArmR2->SetStaticMesh(CMesh7.Object);
	ArmR2->SetRelativeLocation(FVector(0,-2.5,-20));

	// (X=-30.000000,Y=20.000000,Z=-25.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh8(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/Left_leg_high.Left_leg_high"));
	LegL1->SetStaticMesh(CMesh8.Object);
	LegL1->SetRelativeLocation(FVector(0,10,-10.000000));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh9(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/right_leg_high.right_leg_high"));
	LegR1->SetStaticMesh(CMesh9.Object);
	LegR1->SetRelativeLocation(FVector(0,-10,-10));

	// (X=-30.000000,Y=20.000000,Z=-10.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh10(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/Left_leg_low.Left_leg_low"));
	LegL2->SetStaticMesh(CMesh10.Object);
	LegL2->SetRelativeLocation(FVector(0.000000,0,-15));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh11(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/right_leg_low.right_leg_low"));
	LegR2->SetStaticMesh(CMesh11.Object);
	LegR2->SetRelativeLocation(FVector(0,0,-15));

	// (X=-30.000000,Y=20.000000,Z=5.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh12(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/Left_foot.Left_foot"));
	FootL->SetStaticMesh(CMesh12.Object);
	FootL->SetRelativeLocation(FVector(0,0,-15));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh13(TEXT("/Game/LifeDev/Game/Chars/Char07/Parts/Right_foot.Right_foot"));
	FootR->SetStaticMesh(CMesh13.Object);
	FootR->SetRelativeLocation(FVector(0,0,-15));

	Interact->SetRelativeLocation(FVector(0,0,65));
	Interact->SetBoxExtent(FVector(25,20,65));

	AnimFade->SetNewMat();
}

