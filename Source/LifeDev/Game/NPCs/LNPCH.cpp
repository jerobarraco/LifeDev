// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCH.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

ALNPCH::ALNPCH():Super() {
	UseAnim = false;
	IsLocked = true;
	Texts = { NSLOCTEXT("LNPCH", "State0", "Talk") };

	Pelvis = CreateDefaultSubobject<UCQuickMesh>(TEXT("Pelvis"));
	Pelvis->SetupAttachment(Mesh);
	Pelvis->SetUseDynShadow(true);

	Torso = CreateDefaultSubobject<UCQuickMesh>(TEXT("Torso"));
	Torso->SetupAttachment(Pelvis);
	Torso->SetUseDynShadow(true);

	Head = CreateDefaultSubobject<UCQuickMesh>(TEXT("Head"));
	Head->SetupAttachment(Torso);
	Head->SetUseDynShadow(true);

	ArmL1 = CreateDefaultSubobject<UCQuickMesh>(TEXT("ArmL1"));
	ArmL1->SetupAttachment(Torso);
	ArmL1->SetUseDynShadow(true);
		
	ArmR1 = CreateDefaultSubobject<UCQuickMesh>(TEXT("ArmR1"));
	ArmR1->SetupAttachment(Torso);
	ArmR1->SetUseDynShadow(true);

	ArmL2 = CreateDefaultSubobject<UCQuickMesh>(TEXT("ArmL2"));
	ArmL2->SetupAttachment(ArmL1);
	ArmL2->SetUseDynShadow(true);

	ArmR2 = CreateDefaultSubobject<UCQuickMesh>(TEXT("ArmR2"));
	ArmR2->SetupAttachment(ArmR1);
	ArmR2->SetUseDynShadow(true);
	
	LegL1 = CreateDefaultSubobject<UCQuickMesh>(TEXT("LegL1"));
	LegL1->SetupAttachment(Pelvis);
	LegL1->SetUseDynShadow(true);
	
	LegR1 = CreateDefaultSubobject<UCQuickMesh>(TEXT("LegR1"));
	LegR1->SetupAttachment(Pelvis);
	LegR1->SetUseDynShadow(true);

	LegL2 = CreateDefaultSubobject<UCQuickMesh>(TEXT("LegL2"));
	LegL2->SetupAttachment(LegL1);
	LegL2->SetUseDynShadow(true);

	LegR2 = CreateDefaultSubobject<UCQuickMesh>(TEXT("LegR2"));
	LegR2->SetupAttachment(LegR1);
	LegR2->SetUseDynShadow(true);

	FootL = CreateDefaultSubobject<UCQuickMesh>(TEXT("FootL"));
	FootL->SetupAttachment(LegL2);
	FootL->SetUseDynShadow(true);
	
	FootR = CreateDefaultSubobject<UCQuickMesh>(TEXT("FootR"));
	FootR->SetupAttachment(LegR2);
	FootR->SetUseDynShadow(true);

	/// anim fade
	AnimFade->Meshes = {
		Head, Torso, Pelvis,
		ArmL1, ArmL2, ArmR1, ArmR2,
		LegL1, LegL2, LegR1, LegR2,
		FootL, FootR, Mesh
	};
}

void ALNPCH::SetPose(const FNPCHPose& Pose) const {
	Mesh->SetRelativeTransform(Pose.Root);
	Pelvis->SetRelativeTransform(Pose.Pelvis);
	Torso->SetRelativeTransform(Pose.Torso);
	Head->SetRelativeTransform(Pose.Head);
	ArmL1->SetRelativeTransform(Pose.ArmL1);
	ArmL2->SetRelativeTransform(Pose.ArmL2);
	ArmR1->SetRelativeTransform(Pose.ArmR1);
	ArmR2->SetRelativeTransform(Pose.ArmR2);
	LegL1->SetRelativeTransform(Pose.LegL1);
	LegL2->SetRelativeTransform(Pose.LegL2);
	LegR1->SetRelativeTransform(Pose.LegR1);
	LegR2->SetRelativeTransform(Pose.LegR2);
	FootL->SetRelativeTransform(Pose.FootL);
	FootR->SetRelativeTransform(Pose.FootR);

	Interact->SetRelativeLocation(Pose.InteractOrg);
	Interact->SetBoxExtent(Pose.InteractExt);
}

void PrintPose_Loc(const FString& Part, const FVector& V) {
	UE_LOG(LogTemp, Log, TEXT("Pose.%s.SetLocation(FVector(%f, %f, %f));"),
		*Part, V.X, V.Y, V.Z);
}

void PrintPose_Sca(const FString& Part, const FVector& V) {
	UE_LOG(LogTemp, Log, TEXT("Pose.%s.SetScale3D(FVector(%f, %f, %f));"),
		*Part, V.X, V.Y, V.Z);
}

void PrintPose_Rot(const FString& Part, const FRotator& R) {
	UE_LOG(LogTemp, Log, TEXT("Pose.%s.SetRotation(FRotator(%f, %f, %f).Quaternion());"),
		*Part, R.Pitch, R.Yaw, R.Roll);
}

void PrintPose_I(const FString& Part, const FTransform& T) {
	const FVector& L = T.GetLocation();
	const FVector& S = T.GetScale3D();
	const FRotator& R = T.Rotator();
	if (!L.IsZero()) PrintPose_Loc(Part, L);
	if (S != FVector::OneVector) PrintPose_Sca(Part, S);
	if (!R.IsZero()) PrintPose_Rot(Part, R);
}

void ALNPCH::PrintPose() {
	const FVector& IO = Interact->GetRelativeLocation();
	UE_LOG(LogTemp, Log, TEXT("Pose.InteractOrg = FVector(%f, %f, %f);"),
		IO.X, IO.Y, IO.Z);
	
	const FVector& IE = Interact->GetUnscaledBoxExtent();
	UE_LOG(LogTemp, Log, TEXT("Pose.InteractExt = FVector(%f, %f, %f);"),
		IE.X, IE.Y, IE.Z);

	PrintPose_I(TEXT("Root"), Mesh->GetRelativeTransform());
	PrintPose_I(TEXT("Head"), Head->GetRelativeTransform());
	PrintPose_I(TEXT("Pelvis"), Pelvis->GetRelativeTransform());
	PrintPose_I(TEXT("Torso"), Torso->GetRelativeTransform());

	PrintPose_I(TEXT("ArmL2"), ArmL2->GetRelativeTransform());
	PrintPose_I(TEXT("ArmL1"), ArmL1->GetRelativeTransform());
	PrintPose_I(TEXT("ArmR2"), ArmR2->GetRelativeTransform());
	PrintPose_I(TEXT("ArmR1"), ArmR1->GetRelativeTransform());

	PrintPose_I(TEXT("LegL2"), LegL2->GetRelativeTransform());
	PrintPose_I(TEXT("LegL1"), LegL1->GetRelativeTransform());
	PrintPose_I(TEXT("LegR2"), LegR2->GetRelativeTransform());
	PrintPose_I(TEXT("LegR1"), LegR1->GetRelativeTransform());
	
	PrintPose_I(TEXT("FootL"), FootL->GetRelativeTransform());
	PrintPose_I(TEXT("FootR"), FootR->GetRelativeTransform());
}

