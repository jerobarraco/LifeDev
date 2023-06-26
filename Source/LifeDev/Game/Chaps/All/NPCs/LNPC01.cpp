// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01.h"

#include "Interact/CInteract.h"

ALNPC01::ALNPC01():Super() {
	AnimEnabled = false;
	Locked = true;
	Texts = { FText::FromString("Talk") };
	
	Interact->SetRelativeLocation(FVector(32.500000,-30.000000,59.988557));
	Interact->SetBoxExtent(FVector(2.500000,0.000000,70.000000));

	// (X=-30.000000,Y=30.000000,Z=-40.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh1(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Pelvis.Char18-Pelvis"));
	Pelvis->SetStaticMesh(CMesh1.Object);
	Pelvis->SetRelativeLocation(FVector(0,0,50));

	// (X=-30.000000,Y=30.000000,Z=-40.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh2(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Torso.Char18-Torso"));
	Torso->SetStaticMesh(CMesh2.Object);

	// (X=-30.000000,Y=30.000000,Z=-80.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh3(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Head.Char18-Head"));
	Head->SetStaticMesh(CMesh3.Object);
	Head->SetRelativeLocation(FVector(0,0,40));

	// (X=-30.000000,Y=10.000000,Z=-75.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh4(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Arm1.Char18-Arm1"));
	ArmL1->SetStaticMesh(CMesh4.Object);
	ArmL1->SetRelativeLocation(FVector(0.000000,15.000000,35.000000));
	ArmL1->SetRelativeRotation(FRotator(0, 180, 0));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh5(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Arm1.Char18-Arm1"));
	ArmR1->SetStaticMesh(CMesh5.Object);
	ArmR1->SetRelativeLocation(FVector(0.000000,-15.000000,35.000000));

	// (X=-30.000000,Y=12.500000,Z=-55.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh6(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Arm2.Char18-Arm2"));
	ArmL2->SetStaticMesh(CMesh6.Object);
	ArmL2->SetRelativeLocation(FVector(0,-2.5,-20));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh7(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Arm2.Char18-Arm2"));
	ArmR2->SetStaticMesh(CMesh7.Object);
	ArmR2->SetRelativeLocation(FVector(0,-2.5,-20));

	// (X=-30.000000,Y=20.000000,Z=-25.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh8(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Leg1.Char18-Leg1"));
	LegL1->SetStaticMesh(CMesh8.Object);
	LegL1->SetRelativeLocation(FVector(0.000000,-10.000000,-15.000000));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh9(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Leg1.Char18-Leg1"));
	LegR1->SetStaticMesh(CMesh9.Object);
	LegR1->SetRelativeLocation(FVector(0.000000,10.000000,-15.000000));

	// (X=-30.000000,Y=20.000000,Z=-10.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh10(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Leg2.Char18-Leg2"));
	LegL2->SetStaticMesh(CMesh10.Object);
	LegL2->SetRelativeLocation(FVector(0.000000,0.000000,-15.000000));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh11(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Leg2.Char18-Leg2"));
	LegR2->SetStaticMesh(CMesh11.Object);
	LegR2->SetRelativeLocation(FVector(0.000000,0.000000,-15.000000));

	// (X=-30.000000,Y=20.000000,Z=5.000000)
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh12(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Foot.Char18-Foot"));
	FootL->SetStaticMesh(CMesh12.Object);
	FootL->SetRelativeLocation(FVector(0.000000,0.000000,-15.000000));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh13(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/NPC01/Parts/Char18-Foot.Char18-Foot"));
	FootR->SetStaticMesh(CMesh13.Object);
	FootR->SetRelativeLocation(FVector(0.000000,0.000000,-15.000000));
}

void ALNPC01::DiagDone() {
	Destroy();
	Dialogs->OnDone.RemoveAll(this);
	Inventory->Mod("C1", 1);
}

void ALNPC01::TriggerLocked_Implementation() {
	Super::TriggerLocked_Implementation();
	Dialogs->OnDone.AddUniqueDynamic(this, &ALNPC01::DiagDone);
	Dialogs->AddId("N01.0");
	
	// TODO add dialog for join the party
	// and reward card
	// TODO reward new card. 
	// TODO finish the chapter after that
}
