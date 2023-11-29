// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
// Probably copyright of Tom Looman too based on https://github.com/tomlooman/ActionRoguelike/blob/master/Source/ActionRoguelike/Private/Components/SSignificanceComponent.cpp
 
#include "CSignificance.h"

#include "SignificanceManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogJSigComp, Log, Log);

// Allows to force significance on all classes to quickly compare the performance differences as if the system was disabled.
static float GSigOverride = -1;
static FAutoConsoleVariableRef CVarSignificanceManager_SigOverride(
	TEXT("SigMan.SigOverride"),
	GSigOverride,
	TEXT("Force significance on all managed objects. -1 is default, 0-4 is hidden, lowest, medium, highest.\n"),
	ECVF_Cheat
);

UCSignificance::UCSignificance():Super() {
	PrimaryComponentTick.bCanEverTick = false;
	Super::SetAutoActivate(true);
}

void UCSignificance::Activate(bool bReset) {
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs"), __func__);
	Super::Activate(bReset);
	Register();
}

void UCSignificance::Deactivate() {
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs"), __func__);
	Unregister();
	Super::Deactivate();
}

void UCSignificance::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Unregister(); // important or the sigmanager will leak and then crash :) (according to their docs)
	Super::EndPlay(EndPlayReason);
}

void UCSignificance::Register() {
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs"), __func__);

	USignificanceManager* const Man = USignificanceManager::Get(GetWorld());
	if (!IsValid(Man)) return;

	AActor* const Owner = GetOwner();
	if (!IsValid(Owner)) return;
	const FName Tag = Owner->GetClass()->GetFName();

	// this is lame, but it's how it works
	auto lCalculate = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& Viewpoint) -> float
	{
		if (!IsValid(this)) return -1;
		return Calculate(ObjectInfo, Viewpoint);
	};
	
	auto lPostUpdate = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, float Old, float New, bool bFinal)
	{
		if (!IsValid(this)) return;
		PostUpdate(ObjectInfo, Old, New, bFinal);
	};
	
	// Register
	Man->RegisterObject(this, Tag, lCalculate, USignificanceManager::EPostSignificanceType::Sequential, lPostUpdate);
}

void UCSignificance::Unregister() {
	USignificanceManager* const Man = USignificanceManager::Get(GetWorld());
	if (!IsValid(Man)) return;

	Man->UnregisterObject(this);
}

float UCSignificance::Calculate(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& Viewpoint) {
	if (GSigOverride >= 0.0f)
	{
		return GSigOverride;
	}

	AActor* const Actor = GetOwner();
	if (IsOffWhenHidden && Actor && Actor->IsHidden())
	{
		return static_cast<float>(ESignificance::Off);
	}

	if (Actor && RenderSinceMax >= 0 && !Actor->WasRecentlyRendered(RenderSinceMax)) {
		UE_LOG(LogJSigComp, Verbose, TEXT("Actor offscreen for too long. Now is off. name=%s"), *GetNameSafe(Actor));
		return static_cast<float>(ESignificance::Off);
	}

	// Use Actor implemented override if present.
	// otherwise we will calculate it here
	if (CalcSignificance.IsBound()) {
		const float Sig = CalcSignificance.Execute(Viewpoint);
		return Sig;
	}

	// use overriden location if set. otherwise use the actor's one
	FVector Origin;
	if (CalcLocation.IsBound()) {
		Origin = CalcLocation.Execute();	
	} else if (Actor) {
		Origin = Actor->GetActorLocation();
	}

	// calculate using distances
	const float DistSqr = (Origin - Viewpoint.GetLocation()).SizeSquared();
	const float Sig = GetDistanceSignificance(DistSqr);
	
	UE_LOG(LogJSigComp, Verbose, TEXT("Calculated significance. distsqr=%5.3f, sig=%5.3f"), DistSqr, Sig);
	return Sig;

	// TODO visibility by
	//	2- Is actually visible (some trace with visibility channel)
	
	// FCollisionQueryParams Params;
	// Params.Add(Actor);
	
	// need to use this, unfortunately, because this variable doesn't exist otherwise
	// #if !(UE_BUILD_TEST || UE_BUILD_SHIPPING)
	// Params.bDebugQuery = true;
	// #endif

	// UWorld* World = GetWorld();
	// if (!World) return Sig;
	// FHitResult Hit;
	// World->LineTraceSingleByChannel( Hit, Viewpoint, Origin, ECC_Visibility);
	
	// return Sig;
}

void UCSignificance::PostUpdate(USignificanceManager::FManagedObjectInfo* Info, float OldSig, float Sig, bool Final) {
	const bool Equals = FMath::IsNearlyEqual(OldSig, Sig);
	if (Equals) return;

	Significance = static_cast<ESignificance>(FMath::FloorToInt32(Sig));
	const AActor* Owner = GetOwner();
	UE_LOG(LogJSigComp, Log, TEXT("Significance changed. sig=%i owner =%s"), Significance, *GetNameSafe(Owner));

	/// updates
	UpdateTicks();

	// finally notify (at the end, given the side effects)
	OnChanged.Broadcast(Significance);
}

float UCSignificance::GetDistanceSignificance(float DistSqr) {
	const int32 Num = DistanceSqr.Num();
	if (Num == 0) {
		UE_LOG(LogJSigComp, Warning, TEXT("CSignificance: No distance thresholds set in %s."), *GetNameSafe(GetOwner()));
		return static_cast<float>(ESignificance::High);
	}

	// by default is hidden
	ESignificance Sig = ESignificance::Off;

	TArray<ESignificance> Sigs;
	DistanceSqr.GetKeys(Sigs);
	const int32 SigNum = Sigs.Num();
	for (int32 i = 0; i<SigNum; ++i) {
		const ESignificance& ISig = Sigs[i];
		// avoid going back. given the significances can be unordered
		if (ISig < Sig) continue;

		// check distance, and update
		const float SigDistSqr = DistanceSqr[ISig];
		if (DistSqr <= SigDistSqr) {
			Sig = ISig;
		}
	}
	
	return static_cast<float>(Sig);
}

void UCSignificance::UpdateTicks() {
	if (!TickIntervals.Contains(Significance)) return;

	AActor* const Owner = GetOwner();
	if (!IsValid(Owner)) return;
	
	const float Interval = TickIntervals[Significance];
	const bool TickEnabled = Interval>=0;
	Owner->SetActorTickInterval(Interval);
	Owner->SetActorTickEnabled(TickEnabled);

	for (UActorComponent* const C: Comps) {
		if (!IsValid(C)) continue;
		C->SetComponentTickInterval(Interval);
		C->SetComponentTickEnabled(TickEnabled);
	}
}
