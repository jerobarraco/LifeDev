// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
 
#include "CSignificance.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

#include "SignificanceManager.h"
DEFINE_LOG_CATEGORY_STATIC(LogJSigComp, Log, Log);

// Allows to force significance on all classes to quickly compare the performance differences as if the system was disabled.
static float GSigOverride = -1;
static FAutoConsoleVariableRef CVarSignificanceManager_SigOverride(
	TEXT("JSig.SigOverride"),
	GSigOverride,
	TEXT("Force significance on all managed objects. -1 is default, 0-4 is hidden, lowest, medium, highest.\n"),
	ECVF_Cheat
);

UCSignificance::UCSignificance():Super() {
	PrimaryComponentTick.bCanEverTick = false;
	Super::SetAutoActivate(true);
}

void UCSignificance::Activate(const bool Reset) {
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs o=%s"), __func__, *GetNameSafe(GetOwner()));
	const bool WasActive = IsActive();
	// some objects use this.
	if (!Reset && WasActive) return;

	Super::Activate(Reset);
	if (LIKELY(!WasActive)) Register(); // ensure no-double registration.
}

void UCSignificance::Deactivate() {
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs o=%s"), __func__, *GetNameSafe(GetOwner()));
	Unregister();
	Super::Deactivate();
}

void UCSignificance::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	// Unregister(); call deactivate better.
	Deactivate(); // important or the sigmanager will leak and then crash :) (according to ue docs)
	
	CompsActivate.Empty();
	CompsTicks.Empty();
	Super::EndPlay(EndPlayReason);
}

void UCSignificance::Register() {
	const AActor* const Owner = GetOwner();
	const FString& Name = GetNameSafe(Owner);
	const FName Tag(Name);
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs name=%s"), __func__, *Name);

	// don't even try if it's on a cook commandlet or not playing
#if WITH_EDITOR
	if (UNLIKELY((GEditor && !GEditor->IsPlayingSessionInEditor()) || IsRunningCommandlet())) {
		UE_LOG(LogJSigComp, Verbose, TEXT("%hs Can't register. Wrong play mode. Stop. Name=%s"), __func__, *Name);
		return;
	}
#endif

	// don't register if it doesn't have an owner
	if (UNLIKELY(!IsValid(Owner))) {
		UE_LOG(LogJSigComp, Warning, TEXT("%hs Can't register. invalid owner. Stop."), __func__);
		return;
	}

	USignificanceManager* const Man = USignificanceManager::Get(GetWorld());
	if (UNLIKELY(!IsValid(Man))) {
		// note this way of checking for runningcommandlet will not add running cost on shipping builds. and almost none when not cooking.
		UE_CLOG(LIKELY(!IsRunningCookCommandlet()), LogJSigComp, Warning, TEXT("%hs Can't register."
			" Can't obtain the significance manager. Stop. Name=%s"), __func__, *Name);
		return;
	}

	if (!IsValid(Origin))
		Origin = Owner->GetRootComponent();

	UE_LOG(LogJSigComp, Verbose, TEXT("%hs name=%s origin=%s"), __func__, *Name, *GetNameSafe(Origin));
	// this is lame, but it's how it works
	auto LamCalculate = [&] (USignificanceManager::FManagedObjectInfo* const ObjectInfo,
	const FTransform& Viewpoint) -> float {
		if (UNLIKELY(!IsValid(this))) return -1;

		return Calculate(ObjectInfo, Viewpoint);
	};

	auto PostUpdate = [&] (USignificanceManager::FManagedObjectInfo* const ObjectInfo,
	const float Old, const float New, const bool Final) {
		if (UNLIKELY(!IsValid(this))) return;
		Update(ObjectInfo, Old, New, Final);
	};

	// Register
	const USignificanceManager::EPostSignificanceType Type =
		IsConcurrent ? USignificanceManager::EPostSignificanceType::Concurrent
			: USignificanceManager::EPostSignificanceType::Sequential;
	Man->RegisterObject(this, Tag, LamCalculate, Type, PostUpdate);
}

void UCSignificance::Unregister() {
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs %s"), __func__, *GetNameSafe(GetOwner()));

	USignificanceManager* const Man = USignificanceManager::Get(GetWorld());
	if (LIKELY(!IsValid(Man))) return;

	Origin = nullptr; // free reference
	Man->UnregisterObject(this);
}

float UCSignificance::Calculate(USignificanceManager::FManagedObjectInfo* const ObjectInfo,
const FTransform& Viewpoint) {
	if (UNLIKELY(GSigOverride >= 0.0f))
		return GSigOverride;

	const AActor* const Owner = GetOwner();
	if (IsOffIfHidden && Owner && Owner->IsHidden())
		return static_cast<float>(ESigValue::Off);

	// test occlusion BEFORE offscreen
	// i was tempted to believe i will save time.
	// but in truth it will contradict the occlusion and return low even if occluded
	if (TestOcclusion && Owner) {
		if (IsOccluded(Owner, Viewpoint)) {
			UE_LOG(LogJSigComp, Verbose, TEXT("%hs. Actor occluded. Now is off/low. name=%s"),
				__func__, *GetNameSafe(Owner));
		
			return static_cast<float>( IsOffIfOccluded ? ESigValue::Off : ESigValue::Low);
		}
	}
	
	// test offscreen
	if ((OffscreenTimeMax >= 0.0f) & (Owner && !Owner->WasRecentlyRendered(OffscreenTimeMax))) {
		UE_LOG(LogJSigComp, Verbose, TEXT("%hs. Actor offscreen for too long. Now is off/low. name=%s"),
			__func__, *GetNameSafe(Owner));
		
		return static_cast<float>( IsOffIfOffscreen ? ESigValue::Off : ESigValue::Low );
	}

	// Use Actor implemented override if present.
	// otherwise we will calculate it here
	if (CalcSignificance.IsBound())
		return static_cast<float>(CalcSignificance.Execute(Viewpoint));

	// use overriden location if set. otherwise use the actor's one
	FVector OrgLoc;
	if (CalcLocation.IsBound())
		OrgLoc = CalcLocation.Execute();
	else if (Origin)
		OrgLoc = Origin->GetComponentLocation();
	else {
		UE_LOG(LogJSigComp, Warning, TEXT("%hs Warning: Could not obtain the origin location."
			"No origin and no CalcLocation. Stop."), __func__);
		return 0;
	}

	// calculate using distances
	const float DistSqr = FVector::DistSquared(OrgLoc, Viewpoint.GetLocation());
	// might be a bit slower since it creates an intermediate fvector. but who knows.  
	// const float DistSqr1 = (OrgLoc - Viewpoint.GetLocation()).SizeSquared();
	const float Sig = GetDistanceSignificance(DistSqr);

	UE_LOG(LogJSigComp, Verbose, TEXT("%hs Calculated significance. distsqr=%5.3f, sig=%5.3f"),
		__func__, DistSqr, Sig);
	return Sig;
}

float UCSignificance::GetDistanceSignificance(const float DistSqr) {
	const int32 Num = DistanceSqr.Num();
	if (UNLIKELY(Num == 0)) {
		UE_LOG(LogJSigComp, Warning, TEXT("%hs No distance thresholds set in %s."),
			__func__, *GetNameSafe(GetOwner()));
		return static_cast<float>(ESigValue::High);
	}

	// by default is hidden
	ESigValue Sig = ESigValue::Off;

	TArray<ESigValue> Sigs;
	DistanceSqr.GetKeys(Sigs);
	const int32 SigNum = Sigs.Num();
	for (int32 i = 0; i<SigNum; ++i) {
		const ESigValue& ISig = Sigs[i];
		// avoid going back. given the significances can be unordered
		if (UNLIKELY(ISig < Sig)) continue;

		// check distance, and update
		const float SigDistSqr = DistanceSqr[ISig];
		if (DistSqr <= SigDistSqr)
			Sig = ISig;
	}
	
	return static_cast<float>(Sig);
}

bool UCSignificance::IsOccluded(const AActor* const Owner, const FTransform& Viewpoint) const {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return true;
	
	const FVector& Start = Viewpoint.GetLocation();
	const FVector& End = Owner->GetActorLocation();
	FCollisionQueryParams Params;
	
#if !(UE_BUILD_TEST || UE_BUILD_SHIPPING)
	Params.bDebugQuery = true;
	// DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.f, false, 1.f); // annoying
#endif

	FHitResult Hit;
	World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	const AActor* const HitActor = Hit.GetActor();
	// no need to check if either is valid. if it's different it's different.
	const bool Occluded = HitActor != Owner;
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs Occluded=%i o=%s hit=%s"),
		__func__, Occluded, *GetNameSafe(Owner), *GetNameSafe(HitActor));

	return Occluded;
}

void UCSignificance::Update(USignificanceManager::FManagedObjectInfo* const Info,
const float OldSig, const float Sig, const bool Final) {
	const uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();
	const ESigValue NewSig = static_cast<ESigValue>(FMath::FloorToInt32(Sig));
	// don't trust "old" and "sig", use the actually stored. to ensure proper initialization.
	// const bool Equals = FMath::IsNearlyEqual(OldSig, Sig);
	const bool Equals = NewSig == Significance; 
	if (LIKELY(Equals)) return; // return if not changed.

	SignificanceOld = Significance;
	Significance = NewSig;

	UE_CLOG(Debug, LogJSigComp, Log, TEXT("%hs threadId=%i sig=%i owner=%s"),
		__func__, ThreadId, Significance, *GetNameSafe(GetOwner()));

	/// Finish it!!
	// Make sure to call ApplyUpdate on the game thread.
	if (IsInGameThread()) // thanks ue for these super helpful functions
		ApplyUpdate();
	else {
		AsyncTask(ENamedThreads::GameThread, [this] {
			ApplyUpdate();
		});
	}
}

void UCSignificance::ApplyUpdate() {
	/// updates.
	/// This function is called on the game thread (ensured by the caller).
	/// Calling these Update* functions here would be less performant than in other threads.
	/// But it's also more stable, it could crash on another thread.
	UpdateTicks();
	UpdateActivate();
	UpdateHidden();

	// finally notify (at the end, given the possible side effects on the bound delegate client)
	// make sure to notify on the game thread.
	// clients should not worry about the thread and probably assume it's the game thread.
	OnChanged.Broadcast(Significance, SignificanceOld);
}

void UCSignificance::UpdateTicks() {
	if (UNLIKELY(!TickIntervals.Contains(Significance))) return;

	AActor* const Owner = GetOwner();
	if (UNLIKELY(!IsValid(Owner))) return;

	const float PreInterval = TickIntervals[Significance];
	const bool TickEnabled = PreInterval>=0;
	// avoid setting it to -1 if it's going to be disabled.
	const float Interval = TickEnabled ? PreInterval : 999999999;
	
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs Update ticks. Interval=%f Enabled=%i Obj=%s"),
		__func__, Interval, TickEnabled, *GetNameSafe(Owner));

	/// Ticks
	// Don't mess with tick enabled
	
	Owner->SetActorTickInterval(Interval);// Owner

	/// Components
	for (UActorComponent* const C: CompsTicks) {
		if (UNLIKELY(!IsValid(C))) continue;

		// necessary, not enough. when reactivating ticks, it doesn't really update the appropriate value.
		C->SetComponentTickInterval(Interval);
		// this fixes issues when disabling ticks (could happen on CompsActivate)
		C->PrimaryComponentTick.UpdateTickIntervalAndCoolDown(Interval);

		// don't: this will break all the anims and others as it breaks the tick completely
		// C->PrimaryComponentTick.bCanEverTick = TickEnabled;
		// C->SetComponentTickEnabled(TickEnabled); // this will break all anims
		// use this C->PrimaryComponentTick.SetTickFunctionEnable(TickEnabled);
	}
}

void UCSignificance::UpdateActivate() {
	const bool IsActive = Significance != ESigValue::Off;
	for (UActorComponent* const C: CompsActivate) {
		if (UNLIKELY(!IsValid(C))) continue;
		C->SetActive(IsActive, false); // don't reset.
	}
}

void UCSignificance::UpdateHidden() {
	// https://forums.unrealengine.com/t/set-visibility-does-it-help-with-texture-memory-and-other-optimization/138761/3?u=nande
	// according to that ^, setvis affects the editor, which is not exactly what i want. but is kinda the same on runtime.
	// a reason for not using setHidden is that there's a flag (OffWhenHidden) that collides with this intention.
	// though im not sure....
	// const bool IsActive = Significance != ESigValue::Off;
	const bool IsHidden = Significance == ESigValue::Off;
	for (USceneComponent* const C: CompsHide) {
		if (UNLIKELY(!IsValid(C))) continue;
		// C->SetVisibility(IsActive); // this is for the editor only.
		C->SetHiddenInGame(IsHidden);
	}
}
