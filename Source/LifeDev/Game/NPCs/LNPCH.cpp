// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCH.h"

#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

ALNPCH::ALNPCH():Super() {
	UseAnim = false;
	Locked = true;
	Texts = { FText::FromString("Talk") };

	Pelvis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pelvis"));
	Pelvis->SetupAttachment(Mesh);

	Torso = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torso"));
	Torso->SetupAttachment(Pelvis);

	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Torso);

	ArmL1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmL1"));
	ArmL1->SetupAttachment(Torso);
		
	ArmR1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmR1"));
	ArmR1->SetupAttachment(Torso);

	ArmL2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmL2"));
	ArmL2->SetupAttachment(ArmL1);

	ArmR2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmR2"));
	ArmR2->SetupAttachment(ArmR1);
	
	LegL1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegL1"));
	LegL1->SetupAttachment(Pelvis);
	
	LegR1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegR1"));
	LegR1->SetupAttachment(Pelvis);

	LegL2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegL2"));
	LegL2->SetupAttachment(LegL1);

	LegR2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LegR2"));
	LegR2->SetupAttachment(LegR1);

	FootL = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootL"));
	FootL->SetupAttachment(LegL2);
	
	FootR = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FootR"));
	FootR->SetupAttachment(LegR2);

	/// anim fade
	AnimFade->Meshes = {
		Head, Torso, Pelvis,
		ArmL1, ArmL2, ArmR1, ArmR2,
		LegL1, LegL2, LegR1, LegR2,
		FootL, FootR, Mesh
	};
}

void ALNPCH::SetPose(const FNPCHPose& Pose) {
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

void ALNPCH::BeginPlay() {
	Super::BeginPlay();
	AnimFade->CreateMaterial();
}

void PrintPose_Loc(const char16_t* Part, const FVector& V) {
	UE_LOG(LogTemp, Log, TEXT("Pose.%s.SetLocation(FVector(%f, %f, %f));"),
		Part, V.X, V.Y, V.Z);
}

void PrintPose_Sca(const char16_t* Part, const FVector& V) {
	UE_LOG(LogTemp, Log, TEXT("Pose.%s.SetScale3D(FVector(%f, %f, %f));"),
		Part, V.X, V.Y, V.Z);
}

void PrintPose_Rot(const char16_t* Part, const FRotator& R) {
	UE_LOG(LogTemp, Log, TEXT("Pose.%s.SetRotation(FRotator(%f, %f, %f).Quaternion());"),
		Part, R.Pitch, R.Yaw, R.Roll);
}

void PrintPose_I(const char16_t* Part, const FTransform& T) {
	const FVector& L = T.GetLocation();
	const FVector& S = T.GetScale3D();
	const FRotator& R = T.Rotator();
	if (!L.IsZero()) PrintPose_Loc(Part, L);
	if (S != FVector::OneVector) PrintPose_Sca(Part, S);
	if (!R.IsZero()) PrintPose_Rot(Part, R);
}

void ALNPCH::PrintPose() {
	// TODO the interacts
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

