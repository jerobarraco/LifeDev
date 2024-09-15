// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "PIGhost.h"

#include "CGhostAxis.h"
#include "CQuickMesh.h"
#include "Pool.h"
#include "Interact/Animator/CAnimator.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JSig/CSignificance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

	AnimBase = CreateDefaultSubobject<UCAnimator>(TEXT("AnimBase"));
	AnimBase->Duration = 1000;
	AnimBase->IsLooping = true;
	AnimBase->Curve = nullptr;
	AnimBase->SetComponentTickInterval(1/60);
	AnimBase->SetAutoActivate(false);

	AnimFade = CreateDefaultSubobject<UCAnimatorMix>(TEXT("AnimFade"));
	AnimFade->MatFEnd = .75; // don't want to reach 1
	AnimFade->MatFStart = 0;
	AnimFade->Duration = .6;

	Sig = CreateDefaultSubobject<UCSignificance>(TEXT("Sig"));
	Sig->SetAutoActivate(false);
	Sig->CompsTicks = {
		AnimBase, AxisX, AxisY, AxisZ,
		AxisX->Anim, AxisY->Anim, AxisZ->Anim,
	};
	Sig->OffscreenTimeMax = -1;
	Sig->TickIntervals[ESigValue::High] = 1/60;
	Sig->TickIntervals[ESigValue::Med] = 1/30;
	Sig->TickIntervals[ESigValue::Low] = 1/20;
}

void APIGhost::SetActive(const bool Act) {
	Sig->SetActive(Act);

	UActorComponent* const Cmps[] = {AxisX, AxisY, AxisZ, AnimBase};
	for (UActorComponent* const C: Cmps) {
		if (!C) continue;
		C->SetActive(Act);
	}
}

void APIGhost::BeginPlay() {
	Super::BeginPlay();
	AxisX->Anim->OnUpdate.AddUniqueDynamic(this, &APIGhost::RotUpX);
	AxisY->Anim->OnUpdate.AddUniqueDynamic(this, &APIGhost::RotUpY);
	AxisZ->Anim->OnUpdate.AddUniqueDynamic(this, &APIGhost::RotUpZ);

	AxisX->OnUpdate.AddUniqueDynamic(this, &APIGhost::PosUpX);
	AxisY->OnUpdate.AddUniqueDynamic(this, &APIGhost::PosUpY);
	AxisZ->OnUpdate.AddUniqueDynamic(this, &APIGhost::PosUpZ);

	// Create instance, sets it to the mesh, AND store in the anim.
	AnimFade->Mat = Mesh->CreateDynamicMaterialInstance(
		0, Mesh->GetMaterial(0));

	AnimBase->OnUpdate.AddUniqueDynamic(this, &APIGhost::BaseUp);
	Reset();
}

void APIGhost::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	SetActive(false);
	// TODO unbind
	
	Super::EndPlay(EndPlayReason);
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

	BaseUp(0,0);

	SetActive(true);
	AnimFade->PlaySet(false, false, false);
	SetReturnTimer();
}

void APIGhost::SetReturnTimer() {
	const UWorld* const World = GetWorld();
	if (!World) return;

	const float LifeTime = FMath::FRandRange(LifeTimeMin, LifeTimeMax);
	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, this, &APIGhost::FadeAndReturn, LifeTime);
}

void APIGhost::FadeAndReturn() {
	AnimFade->OnEnd.AddUniqueDynamic(this, &APIGhost::Return);
	AnimFade->PlaySet(true, false, false);
}

void APIGhost::Return() {
	AnimFade->OnEnd.RemoveAll(this);

	UPooler* const Pooler = UPooler::Instance(this);
	if (!Pooler) {
		Destroy();
		return;
	}
	
	Pooler->Return(this);
}

void APIGhost::BaseUp(const float Progress, const float Alpha) {
	if (!IsValid(Target)) {
		Target = UGameplayStatics::GetActorOfClass(this, TargetClass);
		if (!IsValid(Target)) return;
	}

	TgtPos = Target->GetActorLocation();
	AimPos = TgtPos + OffPos + OffRot.RotateVector(OffDist);

	const UWorld* const World = GetWorld();

	if (Debug) {
		DrawDebugSphere(World, TgtPos, 3, 12, FColor::Emerald, false, -1, 0, 2);
		DrawDebugLine(World, TgtPos, AimPos, FColor::Cyan, false, -1, 0, 2);
	}

	AxisX->SetTarget(AimPos.X);
	AxisY->SetTarget(AimPos.Y);
	AxisZ->SetTarget(AimPos.Z);

	const FRotator ActRotNew =
		ActRotOff +
		UKismetMathLibrary::FindLookAtRotation(ActPosOld, ActPos);
	ActRot = FMath::RInterpTo(ActRot, ActRotNew,
		World->GetDeltaSeconds(), ActRotSpeed);

	SetActorLocationAndRotation(ActPos, ActRot, false, nullptr, ETeleportType::ResetPhysics);
	ActPosOld = ActPos;
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
