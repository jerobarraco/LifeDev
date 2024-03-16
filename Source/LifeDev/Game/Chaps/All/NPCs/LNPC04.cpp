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

	AnimFade->SetNewMat();

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
	
	// PoseScold = PoseBase;
}

