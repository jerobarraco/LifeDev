// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "Pool.h"

#include "TimerManager.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogJPool, Log, All);

bool UPool::Spawn() {
	UE_LOG(LogJPool, Verbose, TEXT("%hs."), __func__);
	
	static FActorSpawnParameters P;
	P.bNoFail = true;
	P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return false;
	
	AActor* const Actor = World->SpawnActor(ItemType, 0, 0, P);
	if (UNLIKELY(!IsValid(Actor))) {
		UE_LOG(LogJPool, Warning, TEXT("%hs Could not spawn the actor. Stop."), __func__);
		return false;
	}

	Ready.Add(Actor);
	Actor->SetActorHiddenInGame(true);

	return true;
}

bool UPool::Remove() {
	UE_LOG(LogJPool, Verbose, TEXT("%hs."), __func__);
	
	// always remove at end. we could use RemoveAtSwap, but it will probably do the same arithmetic.
	const int32 I = Ready.Num() -1;
	if (UNLIKELY(I < 0)) return false;

	AActor* const A = Ready[I];
	if (LIKELY(IsValid(A))) A->Destroy();

	// remove will shrink
	Ready.RemoveAt(I, 1, EAllowShrinking::Yes);
	return true;
}

void UPool::Set(int32 const Max, const TSubclassOf<AActor> Class, bool const InSetTicks,
bool const InCanGrow, int32 const InTrimTime) {
	if (UNLIKELY(!IsValid(Class))) {
		UE_LOG(LogJPool, Warning, TEXT("%hs. Invalid class, not setting anything."), __func__);
		return;
	}
	
	UE_LOG(LogJPool, Log, TEXT("%hs: Max=%i, Class=%s, SetTick=%i, CanGrow=%i, TrimTime=%i"),
		__func__, Max, *Class->GetFName().ToString(), InSetTicks, InCanGrow, InTrimTime);

	// Class changed. ready elements are invalid.
	if (UNLIKELY(IsValid(ItemType) && Class != ItemType)) {
		UE_LOG(LogJPool, Warning, TEXT("%hs: Class changed. Resetting"), __func__);
		Empty();
	}
	
	SetTicks = InSetTicks;
	CanGrow = InCanGrow;
	ItemType = Class;
	ItemMax = FMath::Max(Max, 0); // Clamp at 0
	TrimTime = FMath::Max(InTrimTime, 0); // clamp at 0 

	// add new ones if needed
	Ready.Reserve(ItemMax);
	while (UNLIKELY(Ready.Num() < ItemMax)) {
		if (UNLIKELY(!Spawn())) break; // avoid infinite loops
	}

	// only reduce the size NOW if the trim time is not set
	// this makes sense when calling set on a used pool.
	if (TrimTime > 0) {
		SetTrimTimer(); // force set timer
	} else {
		// reduce size if Set was called with a smaller value
		while (UNLIKELY(Ready.Num() > ItemMax)) {
			if (UNLIKELY(!Remove())) break; // avoid infinite loops 
		}
	}
}

AActor* UPool::Get() {
	UE_LOG(LogJPool, Verbose, TEXT("%hs."), __func__);

	if (Ready.Num()<=0) {
		if (!CanGrow) {
			// verbose, as it's usual to spam Get in order to ensure the max.
			UE_LOG(LogJPool, Verbose,
				TEXT("%hs: Pool is exhausted, and can't grow. so can't return an actor. Stop."
					" ItemType=%s"),
				__func__, *ItemType->GetName());
			return nullptr;
		}
		
		if (UNLIKELY(!Spawn())) return nullptr; // try to add a new one
	}

	AActor* const A = Ready[0];
	const bool CanShrink = Ready.Num()>ItemMax;
	const EAllowShrinking Shrink = CanShrink ? EAllowShrinking::Yes : EAllowShrinking::No;
	// Use Swap since it's faster, and we don't need to keep the order.
	// remove before checking, or we'll get stuck.
	Ready.RemoveAtSwap(0, 1, Shrink);
	if (UNLIKELY(!IsValid(A))) {
		UE_LOG(LogJPool, Warning, TEXT("%hs. Pool gave an invalid actor."
			" Did it died while in the pool (someone referenced it after return, bad). Try again. obj=%s"),
			__func__, *GetNameSafe(A));
		return nullptr; // avoid crash below. "shouldn't happen"(TM)
	}

	A->SetActorHiddenInGame(false);
	A->Reset();

	if (SetTicks) A->SetActorTickEnabled(true);

	UE_LOG(LogJPool, Verbose, TEXT("%hs. Pool gave an actor. obj=%s"),
		__func__, *GetNameSafe(A));

	return A;
}

void UPool::Return(AActor* const Actor) {
	if (UNLIKELY(!IsValid(Actor))) {
		UE_LOG(LogJPool, Warning, TEXT("%hs: Actor was invalid. Skip"), __func__);
		return;
	}

	UE_LOG(LogJPool, Verbose, TEXT("%hs: Actor returned to pool. A=%s"), __func__, *GetNameSafe(Actor));
	Actor->SetActorHiddenInGame(true);
	if (SetTicks) Actor->SetActorTickEnabled(false);

	// add. ensure no duplicates in case someone is doing something weird.
	Ready.AddUnique(Actor);
	
	// reset the timer after removing from the ready list.
	// 1) to enable it.
	// 2) to avoid the previous timer destroying the newly spawned due to timing issues
	SetTrimTimer();
}

void UPool::Empty() {
	UE_LOG(LogJPool, Log, TEXT("%hs, Clearing pool"), __func__);
	
	TArray<TObjectPtr<AActor>> Old = Ready; // make a copy in case someone is doing something weird.
	Ready.Empty(); // empty asap.
	for(TObjectPtr<AActor>& A: Old) {
		if (LIKELY(IsValid(A))) A->Destroy();
	}
}

void UPool::BeginDestroy() {
	Empty();
	UObject::BeginDestroy();
}

void UPool::SetTrimTimer() {
	UE_LOG(LogJPool, Verbose, TEXT("%hs."), __func__);

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	FTimerManager& Timer = World->GetTimerManager();
	Timer.ClearTimer(TrimTimer);
	TrimTimer.Invalidate();

	// don't schedule if not needed. Spawn will schedule if needed.
	if (TrimTime<=0) {
		UE_LOG(LogJPool, Log, TEXT("%hs. Trim time 0. Disabled."), __func__);
		return;
	}
	
	Timer.SetTimer(TrimTimer, this, &UPool::Trim, TrimTime);
}

void UPool::Trim() {
	UE_LOG(LogJPool, Verbose, TEXT("%hs."), __func__);
	if (UNLIKELY(Ready.Num() <= ItemMax)) {
		UE_LOG(LogJPool, Log, TEXT("%hs. Reached max. not trimming anymore."), __func__);
		return;
	}

	Remove();
	SetTrimTimer();
}

UPooler* UPooler::Instance(UObject* const Ctx) {
	if (UNLIKELY(!Ctx)) return nullptr;
	
	const UWorld* const World = Ctx->GetWorld();
	if (UNLIKELY(!World)) return nullptr;

	return World->GetSubsystem<UPooler>();
}

UPool* UPooler::SetPool(int32 const Max, TSubclassOf<AActor> const Class, bool const SetTicks, bool const CanGrow, int32 const TrimTime) {
	UE_LOG(LogJPool, Log, TEXT("%hs. Max=%i, Ticks=%i, CanGrow=%i, TrimTime=%i, Class=%s"),
		__func__, Max, SetTicks, CanGrow, TrimTime, *GetNameSafe(Class));

	// Not using GetPool because i don't save much and could spam a false negative log.
	if (UNLIKELY(!IsValid(Class))) return nullptr; // fix possible crash
	const FName Key = Class->GetFName();
	TObjectPtr<UPool>* pPool = Pools.Find(Key);
	TObjectPtr<UPool> Pool = nullptr;
	if (LIKELY(pPool))
		Pool = *pPool;
	else {
		Pool = NewObject<UPool>(this);
		Pools.Add(Key, Pool);
	}

	if (UNLIKELY(!IsValid(Pool))) {
		UE_LOG(LogJPool, Warning, TEXT("%hs. Failed to add the pool for class=%s. Stop."),
			__func__, *Key.ToString());
		return nullptr;
	}

	// this will set it, or update it if it exists.
	Pool->Set(Max, Class, SetTicks, CanGrow, TrimTime);
	return Pool;
}

void UPooler::RemPool(TSubclassOf<AActor> const Class) {
	UE_LOG(LogJPool, Log, TEXT("%hs. Class=%s"), __func__, *GetNameSafe(Class));

	const FName Key = Class->GetFName();
	const TObjectPtr<UPool>* const PPool = Pools.Find(Key);
	if (UNLIKELY(!PPool)) {
		UE_LOG(LogJPool, Warning, TEXT("%hs. Could not find the pool. Stop"), __func__);
		return;
	}

	TObjectPtr<UPool> const Pool = *PPool;
	Pool->Empty();

	Pools.Remove(Key);
}

UPool* UPooler::GetPool(const TSubclassOf<AActor> Class) {
	if (UNLIKELY(!IsValid(Class))) return nullptr; // fix possible crash

	const FName Key = Class->GetFName();
	TObjectPtr<UPool>* const pPool = Pools.Find(Key);
	if (UNLIKELY(!pPool)) {
		UE_LOG(LogJPool, Warning, TEXT("%hs. Could not find the pool for class=%s"),
			__func__, *Key.ToString());
		return nullptr;
	}

	return *pPool;
}

AActor* UPooler::Get(const TSubclassOf<AActor> Class) {
	UPool* const Pool = GetPool(Class);
	if (UNLIKELY(!Pool)) return nullptr;

	return Pool->Get();
}

void UPooler::Return(AActor* const Actor) {
	if (UNLIKELY(!IsValid(Actor))) { // checking here to avoid problems on Actor->GetClass
		UE_LOG(LogJPool, Warning, TEXT("%hs. Actor was invalid. Skip"), __func__);
		return;
	}

	UPool* const Pool = GetPool(Actor->GetClass());
	if (UNLIKELY(!Pool)) {
		UE_LOG(LogJPool, Warning, TEXT("%hs. Could not find the pool. Destroying the actor."),
			__func__);
		Actor->Destroy();
		return;
	}

	Pool->Return(Actor);
}
