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
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs o=%s"), __func__, *GetNameSafe(GetOwner()));
	// this is (ab)used by the lights. also ensures no-double registration.
	if (!bReset && IsActive()) return;

	Super::Activate(bReset);
	Register();
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
	const FName Tag(GetNameSafe(Owner));
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs tag=%s"), __func__, Tag);

	// don't register if it doesn't have an owner
	if (!IsValid(Owner)) {
		UE_LOG(LogJSigComp, Warning, TEXT("Can't register, invalid owner"));
		return;
	}

	USignificanceManager* const Man = USignificanceManager::Get(GetWorld());
	if (!IsValid(Man)) return;

	// this is lame, but it's how it works
	auto lCalculate = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& Viewpoint) -> float
	{
		if (!IsValid(this)) return -1;
		return Calculate(ObjectInfo, Viewpoint);
	};
	
	auto lPostUpdate = [&](USignificanceManager::FManagedObjectInfo* ObjectInfo, float Old, float New, bool bFinal)
	{
		if (!IsValid(this)) return;
		Update(ObjectInfo, Old, New, bFinal);
	};

	// Register
	const USignificanceManager::EPostSignificanceType Type =
		IsConcurrent ? USignificanceManager::EPostSignificanceType::Concurrent
			: USignificanceManager::EPostSignificanceType::Sequential;
	Man->RegisterObject(this, Tag, lCalculate, Type, lPostUpdate);
}

void UCSignificance::Unregister() {
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs %s"), __func__, *GetNameSafe(GetOwner()));
	USignificanceManager* const Man = USignificanceManager::Get(GetWorld());
	if (!IsValid(Man)) return;

	Man->UnregisterObject(this);
}

float UCSignificance::Calculate(
	USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& Viewpoint) {
	if (GSigOverride >= 0.0f)
		return GSigOverride;

	const AActor* const Owner = GetOwner();
	if (IsOffIfHidden && Owner && Owner->IsHidden()) {
		return static_cast<float>(ESigValue::Off);
	}

	// test occlusion BEFORE offscreen
	// i was tempted to believe i will save time.
	// but in truth it will contradict the occlusion and return low even if occluded
	if (TestOcclusion && Owner) {
		if (IsOccluded(Owner, Viewpoint)) {
			UE_LOG(LogJSigComp, Verbose, TEXT("%hs. Actor occluded. Now is off/low. name=%s"),
				*GetNameSafe(Owner));
		
			return static_cast<float>( IsOffIfOccluded ? ESigValue::Off : ESigValue::Low);
		}
	}
	
	// test offscreen
	if (Owner && OffscreenTimeMax >= 0.0f && !Owner->WasRecentlyRendered(OffscreenTimeMax)) {
		UE_LOG(LogJSigComp, Verbose, TEXT("%hs. Actor offscreen for too long. Now is off/low. name=%s"),
			*GetNameSafe(Owner));
		
		return static_cast<float>( IsOffIfOffscreen ? ESigValue::Off : ESigValue::Low );
	}

	// Use Actor implemented override if present.
	// otherwise we will calculate it here
	if (CalcSignificance.IsBound()) {
		return static_cast<float>(CalcSignificance.Execute(Viewpoint));
	}

	// use overriden location if set. otherwise use the actor's one
	FVector Origin;
	if (CalcLocation.IsBound()) {
		Origin = CalcLocation.Execute();	
	} else if (Owner) {
		Origin = Owner->GetActorLocation();
	}

	// calculate using distances
	const float DistSqr = (Origin - Viewpoint.GetLocation()).SizeSquared();
	const float Sig = GetDistanceSignificance(DistSqr);
	
	UE_LOG(LogJSigComp, Verbose, TEXT("Calculated significance. distsqr=%5.3f, sig=%5.3f"), DistSqr, Sig);
	return Sig;
}

float UCSignificance::GetDistanceSignificance(float DistSqr) {
	const int32 Num = DistanceSqr.Num();
	if (Num == 0) {
		UE_LOG(LogJSigComp, Warning, TEXT("CSignificance: No distance thresholds set in %s."), *GetNameSafe(GetOwner()));
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
		if (ISig < Sig) continue;

		// check distance, and update
		const float SigDistSqr = DistanceSqr[ISig];
		if (DistSqr <= SigDistSqr) {
			Sig = ISig;
		}
	}
	
	return static_cast<float>(Sig);
}

bool UCSignificance::IsOccluded(const AActor* Owner, const FTransform& Viewpoint) const {
	UWorld* const World = GetWorld();
	if (!World) return true;
	
	const FVector& Start = Viewpoint.GetLocation();
	const FVector& End = Owner->GetActorLocation();
	FCollisionQueryParams Params;
	
#if !(UE_BUILD_TEST || UE_BUILD_SHIPPING)
	Params.bDebugQuery = true;
	// DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.f, false, 1.f); // annoying
#endif

	FHitResult Hit;
	World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	AActor* const HitActor = Hit.GetActor();
	// no need to check if either is valid. if it's different it's different.
	const bool Occluded = HitActor != Owner;
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs Occluded=%i o=%s hit=%s"),
		__func__, Occluded, *GetNameSafe(Owner), *GetNameSafe(HitActor));

	return Occluded;
}

void UCSignificance::Update(USignificanceManager::FManagedObjectInfo* Info, float OldSig, float Sig, bool Final) {
	const uint32 ThreadId = FPlatformTLS::GetCurrentThreadId();
	UE_LOG(LogJSigComp, Verbose, TEXT("%hs threadId=%i"), __func__, ThreadId);
	
	ESigValue NewSig = static_cast<ESigValue>(FMath::FloorToInt32(Sig));
	// don't trust "old" and "sig", use the actually stored. to ensure proper initialization.
	// const bool Equals = FMath::IsNearlyEqual(OldSig, Sig);
	const bool Equals = NewSig == Significance; 
	if (Equals) return; // return if not changed.
	
	Significance = NewSig;
	const AActor* const Owner = GetOwner();

	UE_LOG(LogJSigComp, Log, TEXT("Significance changed. sig=%i owner=%s"),
		Significance, *GetNameSafe(Owner));

	/// Finish it!!
	// Make sure to call ApplyUpdate on the game thread.
	if (IsInGameThread()) { // thanks ue for these super helpful functions
		ApplyUpdate();
	} else {
		AsyncTask(ENamedThreads::GameThread, [this] {
			ApplyUpdate();
		});
	}
}

void UCSignificance::ApplyUpdate() {
	/// updates.
	/// This function is called on the game thread.
	/// Calling these Update* functions here would be less performant than in other threads.
	/// But it's also more stable, it could crash on another thread.
	UpdateTicks();
	UpdateActivate();
	UpdateHidden();

	// finally notify (at the end, given the possible side effects on the bound delegate client)
	// make sure to notify on the game thread.
	// clients should not worry about the thread and probably assume it's the game thread.
	OnChanged.Broadcast(Significance);
}


void UCSignificance::UpdateTicks() {
	if (!TickIntervals.Contains(Significance)) return;

	AActor* const Owner = GetOwner();
	if (!IsValid(Owner)) return;
	
	const float Interval = TickIntervals[Significance];
	const bool TickEnabled = Interval>=0;
	UE_LOG(LogJSigComp, Verbose, TEXT("Update ticks. Interval=%f Enabled=%i Obj=%s"),
		Interval, TickEnabled, *GetNameSafe(Owner));

	/// Owner
	// i wonder if i need to do the same round-about way than the components for the actor
	Owner->SetActorTickInterval(Interval);
	Owner->SetActorTickEnabled(TickEnabled);

	/// Components
	// update ticks stuff. unfortunately this code needs a lot of extra stuff to work reliably.
	for (UActorComponent* const C: CompsTicks) {
		if (!IsValid(C)) continue;

		// avoid setting it to -1 if it's going to be disabled anyways.
		if (TickEnabled) {
			// necessary, not enough. when going from off to low, it doesn't really update the appropriate value.
			C->SetComponentTickInterval(Interval);
		}
		
		// optimization: even though SetTickFunction performs a check, but update interval does not.
		// and i don't like the extra work they do.
		// they do too many functions, pointer handling, and vanilla unhelpful "check"s
		// it's prone to crash and i got one.
		if (C->IsComponentTickEnabled() == TickEnabled) continue;

		// this is the appropriate way to disable ticks.
		// it's safe to call with enabled if it's already enabled. the code does that check.
		// also this only gets called on significance change.
		C->PrimaryComponentTick.SetTickFunctionEnable(TickEnabled);
		
		// this is actually necessary, specially when going from off to on.
		// otherwise the interval is not set correctly.
		C->PrimaryComponentTick.UpdateTickIntervalAndCoolDown(Interval);

		// don't: this will break all the anims and others as it breaks the tick completely
		// C->PrimaryComponentTick.bCanEverTick = TickEnabled;
		// C->SetComponentTickEnabled(TickEnabled); // this will break all anims
	}
}

void UCSignificance::UpdateActivate() {
	const bool IsActive = Significance != ESigValue::Off;
	for (UActorComponent* const C: CompsActivate) {
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
		// C->SetVisibility(IsActive);
		C->SetHiddenInGame(IsHidden);
	}
}
