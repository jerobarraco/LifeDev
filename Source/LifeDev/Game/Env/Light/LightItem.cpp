// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LightItem.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/PointLightComponent.h"

#include "Interact/Animator/CAnimator.h"
#include "Interact/Animator/CAnimatorMix.h"
#include "JSig/CSignificance.h"
#include "Pool.h"
#include "CQuickMesh.h"
#include "JUtils/Misc/JUtilsSys.h"

#include "LifeDev/Game/Env/Ghost/CGhostAxis.h"

ALightItem::ALightItem():Super() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Root);
	Light->SetIntensityUnits(ELightUnits::Lumens);
	Light->SetIntensity(.5);

	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	// TODO
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Engine/BasicShapes/Sphere"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetCastAllShadows(false);
	Mesh->SetRelativeScale3D(FVector(.1));
	Mesh->SetHiddenInGame(!UJUtilsSys::IsEditor());

	// TODO 
	// static ConstructorHelpers::FObjectFinder<UMaterialInterface> CMat(
		// TEXT("/Game/LifeDev/Game/Env/Ghost/GhostFB_DMI.GhostFB_DMI"));
		//TEXT("/Game/LifeDev/Game/Flashback/FlashbackSide_MI.FlashbackSide_MI"));
		// TEXT("/JUtils/Mats/Post/Hidden.Hidden"));
		// TEXT("/Game/LifeDev/Game/Env/Ghost/Ghost_PDMI.Ghost_PDMI"));
	// Mesh->SetMaterial(0, CMat.Object);
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

	// TODO rename fade to set the color. should be a param on light function mat.
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

void ALightItem::SetActive(const bool Act, const bool Reset) {
	Sig->SetActive(Act, Reset);

	UActorComponent* const Cmps[] = {AxisX, AxisY, AxisZ, AnimBase};
	for (UActorComponent* const C: Cmps) {
		if (UNLIKELY(!C)) continue;
		C->SetActive(Act, Reset);
	}
}

void ALightItem::BeginPlay() {
	Super::BeginPlay();
	AxisX->Anim->OnUpdate.AddUniqueDynamic(this, &ALightItem::RotUpX);
	AxisY->Anim->OnUpdate.AddUniqueDynamic(this, &ALightItem::RotUpY);
	AxisZ->Anim->OnUpdate.AddUniqueDynamic(this, &ALightItem::RotUpZ);

	AxisX->OnUpdate.AddUniqueDynamic(this, &ALightItem::PosUpX);
	AxisY->OnUpdate.AddUniqueDynamic(this, &ALightItem::PosUpY);
	AxisZ->OnUpdate.AddUniqueDynamic(this, &ALightItem::PosUpZ);

	// Create instance, sets it to the mesh, AND store in the anim.
	AnimFade->Mat = Mesh->CreateDynamicMaterialInstance(
		0, Mesh->GetMaterial(0));

	AnimBase->OnUpdate.AddUniqueDynamic(this, &ALightItem::BaseUp);
	Reset();
}

void ALightItem::EndPlay(const EEndPlayReason::Type EndPlayReason) {
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

void ALightItem::PostDuplicate(bool Pie) {
	Super::PostDuplicate(Pie);
	// doesn't work. crashes on constructor
	AxisX->RenameComp("X");
	AxisY->RenameComp("Y");
	AxisZ->RenameComp("Z");
}

void ALightItem::Reset() {
	Super::Reset();

	// force reset, so that the PIDs won't make it jump around.
	SetActive(true, true);
	// force update will recalculate the actpos and actrot
	BaseUp(0,0);

	// in case the Light is too far away, or was deactivated before the player teleported, or smth.
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

void ALightItem::SetReturnTimer() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	const float LifeTime = FMath::FRandRange(LifeTimeMin, LifeTimeMax);
	FTimerHandle H;
	FTimerManager& Timer = World->GetTimerManager();
	Timer.SetTimer(H, this, &ALightItem::FadeAndReturn, LifeTime);
}

void ALightItem::FadeAndReturn() {
	AnimFade->OnEnd.AddUniqueDynamic(this, &ALightItem::Return);
	AnimFade->PlaySet(true, false, false);
}

void ALightItem::Return() {
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

void ALightItem::BaseUp(const float Progress, const float Alpha) {
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

void ALightItem::PosUpX(const float Output, const float NewValue) {
	ActPos.X = NewValue;
}
void ALightItem::PosUpY(const float Output, const float NewValue) {
	ActPos.Y = NewValue;
}
void ALightItem::PosUpZ(const float Output, const float NewValue) {
	ActPos.Z = NewValue;
}

void ALightItem::RotUpX(const float Progress, const float Alpha) {
	OffRot.Roll = 360*Alpha;
}

void ALightItem::RotUpY(const float Progress, const float Alpha) {
	OffRot.Pitch = 360*Alpha;
}

void ALightItem::RotUpZ(const float Progress, const float Alpha) {
	OffRot.Yaw = 360*Alpha;
}
