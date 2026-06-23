// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "GhostItem.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Pawn.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

#include "Interact/Animator/CAnimator.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "Pool.h"

#include "CGhostAxis.h"
#include "CQuickMesh.h"
#include "LifeDev/Game/Interact/CLSignificance.h"

AGhostItem::AGhostItem():Super() {
	TargetClass = APawn::StaticClass();
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Env/Ghost/Ghost-03"));
	Mesh->SetStaticMesh(CMesh.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CMat(
		TEXT("/Game/LifeDev/Game/Env/Ghost/GhostFB_DMI.GhostFB_DMI"));
		//TEXT("/Game/LifeDev/Game/Flashback/FlashbackSide_MI.FlashbackSide_MI"));
		// TEXT("/JUtils/Mats/Post/Hidden.Hidden"));
		// TEXT("/Game/LifeDev/Game/Env/Ghost/Ghost_PDMI.Ghost_PDMI"));
	Mesh->SetMaterial(0, CMat.Object);
	// Mesh->SetCustomDepth(true, 1);
	AxisX = CreateDefaultSubobject<UCGhostAxis>(TEXT("AxisX"));
	AxisY = CreateDefaultSubobject<UCGhostAxis>(TEXT("AxisY"));
	AxisZ = CreateDefaultSubobject<UCGhostAxis>(TEXT("AxisZ"));

	// used to update the values. using an anim so that i can use the significance.
	AnimBase = CreateDefaultSubobject<UCAnimator>(TEXT("AnimBase"));
	AnimBase->Duration = 1000;
	AnimBase->IsLooping = true;
	AnimBase->Curve = nullptr;
	AnimBase->SetAutoActivate(false);
	AnimBase->UseLog = false;

	AnimFade = CreateDefaultSubobject<UCAnimatorMix>(TEXT("AnimFade"));
	AnimFade->MatFEnd = .75; // don't want to reach 1
	AnimFade->MatFStart = 0;
	AnimFade->Duration = 1.25;
	AnimFade->MatFName = "Opacity";

	Sig = CreateDefaultSubobject<UCLSignificance>(TEXT("Sig"));
	Sig->SetAutoActivate(false);
	Sig->CompsTicks = {
		AnimBase, AxisX, AxisY, AxisZ,
		AxisX->Anim, AxisY->Anim, AxisZ->Anim,
	};
	Sig->OffscreenTimeMax = -1;
}

void AGhostItem::SetActive(const bool Act, const bool Reset) {
	// fades are somewhere else (on reset and fadeandreturn)
	Sig->SetActive(Act, Reset);

	UActorComponent* const Cmps[] = {AxisX, AxisY, AxisZ, AnimBase};
	for (UActorComponent* const C: Cmps) {
		if (UNLIKELY(!C)) continue;
		C->SetActive(Act, Reset);
	}
}

void AGhostItem::BeginPlay() {
	Super::BeginPlay();
	AxisX->Anim->OnUpdate.AddUniqueDynamic(this, &AGhostItem::RotUpX);
	AxisY->Anim->OnUpdate.AddUniqueDynamic(this, &AGhostItem::RotUpY);
	AxisZ->Anim->OnUpdate.AddUniqueDynamic(this, &AGhostItem::RotUpZ);

	AxisX->OnUpdate.AddUniqueDynamic(this, &AGhostItem::PosUpX);
	AxisY->OnUpdate.AddUniqueDynamic(this, &AGhostItem::PosUpY);
	AxisZ->OnUpdate.AddUniqueDynamic(this, &AGhostItem::PosUpZ);

	// Create instance, sets it to the mesh, AND store in the anim.
	AnimFade->Mat = Mesh->CreateDynamicMaterialInstance(
		0, Mesh->GetMaterial(0));

	AnimBase->OnUpdate.AddUniqueDynamic(this, &AGhostItem::BaseUp);
	Reset();
}

void AGhostItem::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	SetActive(false);
	
	AxisX->Anim->OnUpdate.RemoveAll(this);
	AxisY->Anim->OnUpdate.RemoveAll(this);
	AxisZ->Anim->OnUpdate.RemoveAll(this);

	AxisX->OnUpdate.RemoveAll(this);
	AxisY->OnUpdate.RemoveAll(this);
	AxisZ->OnUpdate.RemoveAll(this);

	AnimBase->OnUpdate.RemoveAll(this);
	
	Super::EndPlay(EndPlayReason);
}

void AGhostItem::PostDuplicate(const bool Pie) {
	Super::PostDuplicate(Pie);
	// doesn't work. crashes on constructor
	AxisX->RenameComp("X");
	AxisY->RenameComp("Y");
	AxisZ->RenameComp("Z");
}

void AGhostItem::Reset() {
	Super::Reset();

	// force reset, so that the PIDs won't make it jump around.
	SetActive(true, true);
	// force update will recalculate the actpos and actrot
	BaseUp(0,0);

	// in case the ghost is too far away, or was deactivated before the player teleported, or smth.
	// teleport closer to the pawn.
	ActPos = AimPos; // aimPos is calculated by BaseUp
	AxisX->SetVal(ActPos.X);
	AxisY->SetVal(ActPos.Y);
	AxisZ->SetVal(ActPos.Z);
	// baseUp calls SetActorLocation, but it's not the correct location. but we need BaseUp to calculate AimPos
	SetActorLocation(ActPos);

	AnimFade->PlaySet(false, false, false);
	SetReturnTimer();
}

void AGhostItem::SetReturnTimer() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	const float LifeTime = FMath::FRandRange(LifeTimeMin, LifeTimeMax);
	FTimerHandle H;
	FTimerManager& Timer = World->GetTimerManager();
	Timer.SetTimer(H, this, &AGhostItem::FadeAndReturn, LifeTime);
}

void AGhostItem::FadeAndReturn() {
	AnimFade->OnEnd.AddUniqueDynamic(this, &AGhostItem::Return);
	AnimFade->PlaySet(true, false, false);
}

void AGhostItem::Return() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	// avoid issues with double returns
	World->GetTimerManager().ClearAllTimersForObject(this);
	
	SetActive(false);
	AnimFade->OnEnd.RemoveAll(this);

	UPooler* const Pooler = UPooler::Instance(this);
	if (UNLIKELY(!Pooler)) {
		Destroy();
		return;
	}
	
	Pooler->Return(this);
}

void AGhostItem::BaseUp(const float Progress, const float Alpha) {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	if (UNLIKELY(!IsValid(Target))) {
		Target = UGameplayStatics::GetActorOfClass(this, TargetClass);
		if (UNLIKELY(!IsValid(Target))) return;
	}

	TgtPos = Target->GetActorLocation();
	AimPos = TgtPos + OffPos + OffRot.RotateVector(OffDist);

	// if (Debug) {
		// DrawDebugSphere(World, TgtPos, 3, 12, FColor::Emerald, false, -1, 0, 2);
		// DrawDebugLine(World, TgtPos, AimPos, FColor::Cyan, false, -1, 0, 2);
	// }

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

void AGhostItem::PosUpX(const float Output, const float NewValue) {
	ActPos.X = NewValue;
}
void AGhostItem::PosUpY(const float Output, const float NewValue) {
	ActPos.Y = NewValue;
}
void AGhostItem::PosUpZ(const float Output, const float NewValue) {
	ActPos.Z = NewValue;
}

void AGhostItem::RotUpX(const float Progress, const float Alpha) {
	OffRot.Roll = 360*Alpha;
}

void AGhostItem::RotUpY(const float Progress, const float Alpha) {
	OffRot.Pitch = 360*Alpha;
}

void AGhostItem::RotUpZ(const float Progress, const float Alpha) {
	OffRot.Yaw = 360*Alpha;
}
