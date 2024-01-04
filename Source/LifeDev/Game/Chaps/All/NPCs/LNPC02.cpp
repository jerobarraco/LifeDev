// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC02.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"

ALNPC02::ALNPC02():Super() {
	UseAnim = false;
	Locked = true;
	Texts = { FText::FromString("...") };

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

	AnimFade->SetNewMat();
}

