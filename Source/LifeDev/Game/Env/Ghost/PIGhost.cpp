// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "PIGhost.h"

#include "CGhostAxis.h"
#include "CQuickMesh.h"
#include "Interact/Animator/CAnimator.h"

APIGhost::APIGhost():Super() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CMesh(
		TEXT("/Game/LifeDev/Game/Env/Ghost/Ghost-03"));
	Mesh->SetStaticMesh(CMesh.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CMat(
		TEXT("/Game/LifeDev/Game/Env/Ghost/Ghost_PDMI.Ghost_PDMI"));
	Mesh->SetMaterial(0, CMat.Object);
	
	AxisX = CreateDefaultSubobject<UCGhostAxis>(TEXT("AxisX"));
	AxisY = CreateDefaultSubobject<UCGhostAxis>(TEXT("AxisY"));
	AxisZ = CreateDefaultSubobject<UCGhostAxis>(TEXT("AxisZ"));
	// TODO csig
	// TODO animmat
	// TODO animbase
}

void APIGhost::BeginPlay() {
	Super::BeginPlay();
	AxisX->Anim->OnUpdate.AddUniqueDynamic(this, &APIGhost::RotUpX);
	AxisY->Anim->OnUpdate.AddUniqueDynamic(this, &APIGhost::RotUpY);
	AxisZ->Anim->OnUpdate.AddUniqueDynamic(this, &APIGhost::RotUpZ);

	AxisX->OnUpdate.AddUniqueDynamic(this, &APIGhost::PosUpX);
	AxisY->OnUpdate.AddUniqueDynamic(this, &APIGhost::PosUpY);
	AxisZ->OnUpdate.AddUniqueDynamic(this, &APIGhost::PosUpZ);

	// TODO anim base
	Mesh->CreateDynamicMaterialInstance(0, Mesh->GetMaterial(0));

	Reset();
}

void APIGhost::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	// TODO unbind
}

void APIGhost::PostDuplicate(bool Pie) {
	Super::PostDuplicate(Pie);
	// doesnt work. crashes on constructor
	AxisX->RenameComp("X");
	AxisY->RenameComp("Y");
	AxisZ->RenameComp("Z");
}

void APIGhost::Reset() {
	Super::Reset();

	ActPos = GetActorLocation();
	ActRot = GetActorRotation();

	AxisX->SetVal(ActPos.X);
	AxisY->SetVal(ActPos.Y);
	AxisZ->SetVal(ActPos.Z);
	
	// TODO base upd 0
	// todo set timer
	// todo set active
	// todo playset animmat
}

void APIGhost::PosUpX(const float Output, const float NewValue) {
	ActPos.X = NewValue;
}
void APIGhost::PosUpY(const float Output, const float NewValue) {
	ActPos.Y = NewValue;
}
void APIGhost::PosUpZ(const float Output, const float NewValue) {
	ActPos.Z = NewValue;
}

void APIGhost::RotUpX(const float Progress, const float Alpha) {
	OffRot.Roll = 360*Alpha;
}
void APIGhost::RotUpY(const float Progress, const float Alpha) {
	OffRot.Pitch = 360*Alpha;
}
void APIGhost::RotUpZ(const float Progress, const float Alpha) {
	OffRot.Yaw = 360*Alpha;
}
