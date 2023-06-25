// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCA01.h"

#include "Interact/CInteract.h"

ALNPCA01::ALNPCA01():Super() {
	AnimEnabled = false;
	Locked = true;
	Texts = { FText::FromString("Talk") };
	
	// static ConstructorHelpers::FObjectFinder<UStaticMesh> CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Char18.Char18"));
	// Mesh->SetStaticMesh(CMesh.Object);
	// Mesh->SetRelativeLocation(FVector(-30.000000,30.000000,10.092946));

	Interact->SetRelativeLocation(FVector(32.500000,-30.000000,59.988557));
	Interact->SetBoxExtent(FVector(2.500000,0.000000,70.000000));

	// (X=-30.000000,Y=30.000000,Z=-40.000000)
	Pelvis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pelvis"));
	Pelvis->SetupAttachment(Mesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh1(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Pelvis.Char18-Pelvis"));
	Pelvis->SetStaticMesh(CMesh1.Object);
	Pelvis->SetRelativeLocation(FVector(0,0,50));

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
	Head->SetRelativeLocation(FVector(0,0,40));

	// (X=-30.000000,Y=10.000000,Z=-75.000000)
	ArmL1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmL1"));
	ArmL1->SetupAttachment(Torso);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh4(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Arm1.Char18-Arm1"));
	ArmL1->SetStaticMesh(CMesh4.Object);
	ArmL1->SetRelativeLocation(FVector(0.000000,15.000000,35.000000));
	ArmL1->SetRelativeRotation(FRotator(0, 180, 0));
	
	ArmR1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmR1"));
	ArmR1->SetupAttachment(Torso);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh5(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Arm1.Char18-Arm1"));
	ArmR1->SetStaticMesh(CMesh5.Object);
	ArmR1->SetRelativeLocation(FVector(0.000000,-15.000000,35.000000));

	// (X=-30.000000,Y=12.500000,Z=-55.000000)
	ArmL2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmL2"));
	ArmL2->SetupAttachment(ArmL1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh6(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Arm2.Char18-Arm2"));
	ArmL2->SetStaticMesh(CMesh6.Object);
	ArmL2->SetRelativeLocation(FVector(0,-2.5,-20));
	ArmR2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmR2"));
	ArmR2->SetupAttachment(ArmR1);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh7(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Arm2.Char18-Arm2"));
	ArmR2->SetStaticMesh(CMesh7.Object);
	ArmR2->SetRelativeLocation(FVector(0,-2.5,-20));

	// (X=-30.000000,Y=20.000000,Z=-25.000000)
	LegL1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegL1"));
	LegL1->SetupAttachment(Pelvis);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh8(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Leg1.Char18-Leg1"));
	LegL1->SetStaticMesh(CMesh8.Object);
	LegL1->SetRelativeLocation(FVector(0.000000,-10.000000,-15.000000));
	
	LegR1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegR1"));
	LegR1->SetupAttachment(Pelvis);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh9(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Leg1.Char18-Leg1"));
	LegR1->SetStaticMesh(CMesh9.Object);
	LegR1->SetRelativeLocation(FVector(0.000000,10.000000,-15.000000));

	// (X=-30.000000,Y=20.000000,Z=-10.000000)
	LegL2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegL2"));
	LegL2->SetupAttachment(LegL1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh10(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Leg2.Char18-Leg2"));
	LegL2->SetStaticMesh(CMesh10.Object);
	LegL2->SetRelativeLocation(FVector(0.000000,0.000000,-15.000000));

	LegR2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegR2"));
	LegR2->SetupAttachment(LegR1);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh11(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Leg2.Char18-Leg2"));
	LegR2->SetStaticMesh(CMesh11.Object);
	LegR2->SetRelativeLocation(FVector(0.000000,0.000000,-15.000000));

	// (X=-30.000000,Y=20.000000,Z=5.000000)
	FootL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootL"));
	FootL->SetupAttachment(LegL2);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh12(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Foot.Char18-Foot"));
	FootL->SetStaticMesh(CMesh12.Object);
	FootL->SetRelativeLocation(FVector(0.000000,0.000000,-15.000000));

	FootR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootR"));
	FootR->SetupAttachment(LegR2);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh13(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Foot.Char18-Foot"));
	FootR->SetStaticMesh(CMesh13.Object);
	FootR->SetRelativeLocation(FVector(0.000000,0.000000,-15.000000));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/Ghost/Ghost_MI.Ghost_MI"));
	FadeMatClass = CMat.Object;	
}

void ALNPCA01::DiagDone() {
	Destroy();
	Dialogs->OnDone.RemoveAll(this);
	// TODO spawn a card instead
	Inventory->Mod("C1", 1);
}

void ALNPCA01::TriggerLocked_Implementation() {
	Super::TriggerLocked_Implementation();
	Dialogs->OnDone.AddUniqueDynamic(this, &ALNPCA01::DiagDone);
	Dialogs->AddId("N01.0");
	
	// TODO add dialog for join the party
	// and reward card
	// TODO reward new card. 
	// TODO finish the chapter after that
}

void ALNPCA01::BeginPlay() {
	Super::BeginPlay();
	// has to be done on begin play or the bp wont save
	// also i think the translucent material breaks the outline shader
	// maybe use a different animation then, maybe a color override
	SetFadeMat();
	Pelvis->SetMaterial(0, FadeMat);
	Torso->SetMaterial(0, FadeMat);
	Head->SetMaterial(0, FadeMat);
	ArmL1->SetMaterial(0, FadeMat);
	ArmR1->SetMaterial(0, FadeMat);
	ArmL2->SetMaterial(0, FadeMat);
	ArmR2->SetMaterial(0, FadeMat);
	FootL->SetMaterial(0, FadeMat);
	FootR->SetMaterial(0, FadeMat);
	LegL1->SetMaterial(0, FadeMat);
	LegL2->SetMaterial(0, FadeMat);
	LegR1->SetMaterial(0, FadeMat);
	LegR2->SetMaterial(0, FadeMat);
}
