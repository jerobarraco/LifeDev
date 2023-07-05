// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC01.h"

#include "Interact/CInteract.h"

ALNPC01::ALNPC01():Super() {
	AnimEnabled = false;
	Locked = true;
	Texts = { FText::FromString("He might attack me...") };

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


	PoseStand.InteractOrg = FVector(2.500000,5.000000,67.500000);
	PoseStand.InteractExt = FVector(20.000000,30.000000,70.000000);
	PoseStand.Pelvis.SetLocation(FVector(0,0,50));
	PoseStand.Head.SetLocation(FVector(0,0,40));
	PoseStand.ArmL1.SetLocation(FVector(0.000000,15.000000,35.000000));
	PoseStand.ArmL1.SetRotation(FRotator(0, 180, 0).Quaternion());
	PoseStand.ArmR1.SetLocation(FVector(0.000000,-15.000000,35.000000));
	PoseStand.ArmL2.SetLocation(FVector(0,-2.5,-20));
	PoseStand.ArmR2.SetLocation(FVector(0,-2.5,-20));
	PoseStand.LegL1.SetLocation(FVector(0.000000,-10.000000,-15.000000));
	PoseStand.LegR1.SetLocation(FVector(0.000000,10.000000,-15.000000));
	PoseStand.LegL2.SetLocation(FVector(0.000000,0.000000,-15.000000));
	PoseStand.LegR2.SetLocation(FVector(0.000000,0.000000,-15.000000));
	PoseStand.FootL.SetLocation(FVector(0.000000,0.000000,-15.000000));
	PoseStand.FootR.SetLocation(FVector(0.000000,0.000000,-15.000000));
	PoseBase = PoseStand;

	// Copy the translation and base rotations
	PoseSit = PoseStand;
	PoseSit.InteractOrg = FVector(15.000000,0.000000,40.000000);
	PoseSit.InteractExt = FVector(40.000000,30.000000,40.000000);
	PoseSit.Pelvis.SetLocation(FVector(-7.635560,0.000000,6.696581));
	PoseSit.Pelvis.SetRotation(FRotator(80.000000,-0.000000,-0.000000).Quaternion());
	PoseSit.Head.SetRotation(FRotator(-33.825845,37.004502,-22.760476).Quaternion());
	PoseSit.Torso.SetRotation(FRotator(-80.000000,-180.000000,180.000000).Quaternion());
	PoseSit.ArmL1.SetRotation(FRotator(-69.999999,180.000000,0.000000).Quaternion());
	PoseSit.ArmL2.SetRotation(FRotator(-17.229397,58.433295,-79.685895).Quaternion());
	PoseSit.ArmR1.SetRotation(FRotator(79.999999,-0.000000,-10.000000).Quaternion());
	PoseSit.ArmR2.SetRotation(FRotator(18.747237,36.005215,-43.858655).Quaternion());
	PoseSit.LegL1.SetRotation(FRotator(60.000000,0.000000,0.000000).Quaternion());
	PoseSit.LegL2.SetRotation(FRotator(-90.000000,-180.000000,180.000000).Quaternion());
	PoseSit.FootL.SetRotation(FRotator(-30.000000,0.000000,0.000000).Quaternion());
	PoseSit.LegR1.SetRotation(FRotator(40.000000,0.000000,0.000000).Quaternion());
	PoseSit.LegR2.SetRotation(FRotator(-50.000000,0.000000,0.000000).Quaternion());
	PoseSit.FootR.SetRotation(FRotator(-20.000000,0.000000,0.000000).Quaternion());

	SetPose(PoseSit);
}

void ALNPC01::SetPoseStand() {
	// TODO do fades
	SetPose(PoseBase);
}

void ALNPC01::SetPoseSit() {
	// do fades here
	SetPose(PoseSit);
}

void ALNPC01::SetVisible(bool Vis) {
	if (Vis) {
		SetPoseSit();
	// TODO implement with ambient sound
	// FGameplayTag Rain = FGameplayTag::RequestGameplayTag(TEXT("Sounds.Rain"));
	// USoundscapeSubsystem* SScape = GetWorld()->GetGameInstance()->GetSubsystem<USoundscapeSubsystem>();
	// SScape->SetState(Rain);
	}
	Interact->SetEnabled(Vis);
	SetActorHiddenInGame(!Vis);
}

void ALNPC01::BeginPlay() {
	Super::BeginPlay();
	if (IsValid(Card)) {
		Card->SetActorHiddenInGame(true);
	}
}

void ALNPC01::DiagStandDone() {
	Dialogs->OnDone.RemoveAll(this);
	if (IsValid(Card)) {
		Card->SetActorHiddenInGame(false);
	}
	Destroy();
	// FGameplayTag Rain = FGameplayTag::RequestGameplayTag(TEXT("Sounds.Rain"));
	// USoundscapeSubsystem* SScape = GetWorld()->GetGameInstance()->GetSubsystem<USoundscapeSubsystem>();
	// SScape->ClearState(Rain);
}

void ALNPC01::DiagSitDone() {
	Dialogs->OnDone.RemoveAll(this);
	SetPoseStand();
	AddActorLocalRotation(FRotator(0, -120, 0));
	Dialogs->OnDone.AddUniqueDynamic(this, &ALNPC01::DiagStandDone);
	Dialogs->AddId("N01.1");
}

void ALNPC01::TriggerLocked_Implementation() {
	Super::TriggerLocked_Implementation();
	Dialogs->OnDone.AddUniqueDynamic(this, &ALNPC01::DiagSitDone);
	Dialogs->AddId("N01.0");
}

