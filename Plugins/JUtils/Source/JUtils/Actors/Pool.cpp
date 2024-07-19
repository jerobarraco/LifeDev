// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "Pool.h"

DEFINE_LOG_CATEGORY_STATIC(LogJPool, Log, Log);

bool UPool::Spawn() {
	UE_LOG(LogJPool, Verbose, TEXT("%hs."), __func__);
	
	static FActorSpawnParameters P;
	P.bNoFail = true;
	P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UWorld* const World = GetWorld();
	if (!World) return false;
	
	AActor* const Actor = World->SpawnActor(ItemType, 0, 0, P);
	if (!IsValid(Actor)) {
		UE_LOG(LogJPool, Warning, TEXT("Could not spawn the actor."));
		return false;
	}

	Ready.Add(Actor);
	Actor->SetActorHiddenInGame(true);

	return true;
}

bool UPool::RemoveOne() {
	UE_LOG(LogJPool, Verbose, TEXT("%hs."), __func__);
	
	// always remove at end. we could use RemoveAtSwap but it will probably do the same arithmetic.
	const int32 I = Ready.Num() -1;
	if (I < 0) return false;

	AActor* const A = Ready[I];
	if (IsValid(A)) {
		A->Destroy();
	}

	// remove will shrink
	Ready.RemoveAt(I, 1, true);
	return true;
}

void UPool::Set(int32 Max, TSubclassOf<AActor> Class, bool InSetTicks, bool InCanGrow, int32 InTrimTime) {
	if (!IsValid(Class)) {
		UE_LOG(LogJPool, Warning, TEXT("%hs. Invalid class, not setting anything."), __func__);
		return;
	}
	
	UE_LOG(LogJPool, Log, TEXT("%hs: Max=%i, Class=%s, SetTick=%i, CanGrow=%i, TrimTime=%i"),
		__func__, Max, *Class->GetFName().ToString(), InSetTicks, InCanGrow, InTrimTime);

	// Class changed. ready elements are invalid.
	if (Class != ItemType) {
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
	while (Ready.Num() < ItemMax) {
		if (!Spawn()) break; // avoid infinite loops
	}

	// only reduce the size NOW if the trim time is not set
	if (TrimTime > 0) {
		SetTrimTimer(); // force set timer
	} else {
		// reduce size if Set was called with a smaller value
		while (Ready.Num() > ItemMax) {
			if (RemoveOne()) break; // avoid infinite loops 
		}
	}
}

AActor* UPool::Get() {
	UE_LOG(LogJPool, Verbose, TEXT("%hs."), __func__);

	// TODO this is crashing somewhere maybe. crash on "ke * get set.audio.VolMusic"
	
	if (Ready.Num()<=0) {
		if (!CanGrow) {
			UE_LOG(LogJPool, Warning, TEXT("Pool is exhausted, and can't grow. so can't return an actor."));
			return nullptr;
		}
		// try to add a new one
		if (!Spawn()) return nullptr;
	}
	
	AActor* const A = Ready[0];
	// don't shrink since it will get returned, hopefully. Use Swap since it's faster and we don't need to keep the order.
	Ready.RemoveAtSwap(0, 1, false);
	A->SetActorHiddenInGame(false);
	A->Reset();
	if (SetTicks) A->SetActorTickEnabled(true);

	UE_LOG(LogJPool, Verbose, TEXT("Pool gave an actor."));

	return A;
}

void UPool::Return(AActor* Actor) {
	if (!IsValid(Actor)) {
		UE_LOG(LogJPool, Warning, TEXT("Return: Actor was invalid."));
		return;
	}

	UE_LOG(LogJPool, Verbose, TEXT("Actor returned to pool."));
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
	UE_LOG(LogJPool, Log, TEXT("Clearing pool"));
	
	TArray<AActor*> Old = Ready; // make a copy in case someone is doing something weird.
	Ready.Empty(); // empty asap.
	for(AActor* const A: Old) {
		A->Destroy();
	}
}

void UPool::BeginDestroy() {
	Empty();
	UObject::BeginDestroy();
}

void UPool::SetTrimTimer() {
	UE_LOG(LogJPool, Verbose, TEXT("%hs."), __func__);

	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	FTimerManager& Timer = World->GetTimerManager();
	Timer.ClearTimer(TrimTimer);
	TrimTimer.Invalidate();

	// don't schedule if not needed. Spawn will schedule if needed.
	if (TrimTime<=0) {
		UE_LOG(LogJPool, Log, TEXT("%hs. Reached max. not trimming anymore."), __func__);
		return;
	}

	if (Ready.Num() <= ItemMax)  {
		UE_LOG(LogJPool, Log, TEXT("%hs. Reached max. not trimming anymore."), __func__);
		return;
	}
	
	Timer.SetTimer(TrimTimer, this, &UPool::Trim, TrimTime);
}

void UPool::Trim() {
	UE_LOG(LogJPool, Verbose, TEXT("%hs."), __func__);
	if (Ready.Num() <= ItemMax) {
		UE_LOG(LogJPool, Log, TEXT("%hs. Reached max. not trimming anymore."), __func__);
		return;
	}

	RemoveOne();
	SetTrimTimer();
}

bool UPooler::AddPool(int32 Max, TSubclassOf<AActor> Class, bool SetTicks, bool CanGrow, int32 TrimTime) {
	const FName Key = Class->GetFName();
	UPool** pPool = Pools.Find(Key);
	UPool* Pool = nullptr;
	if (pPool) {
		Pool = *pPool;
	}else{
		Pool = NewObject<UPool>(this);
		Pools.Add(Key, Pool);
	}

	if (!IsValid(Pool)) {
		UE_LOG(LogJPool, Warning, TEXT("Pooler.AddPool. Failed to add the pool for class=%s"), *Key.ToString());
		return false;
	}

	// this will set it, or update it if it exists.
	Pool->Set(Max, Class, SetTicks, CanGrow, TrimTime);
	return true;
}

void UPooler::RemPool(TSubclassOf<AActor> Class) {
	const FName Key = Class->GetFName();
	
	UPool** const PPool = Pools.Find(Key);
	if (!PPool) {
		UE_LOG(LogJPool, Warning, TEXT("DelPool. Could not find the pool"));
		return;
	}

	UPool* const Pool = *PPool; 
	Pool->Empty();
	
	Pools.Remove(Key);
}

UPool* UPooler::GetPool(TSubclassOf<AActor> Class) {
	if (!IsValid(Class)) return nullptr; // possible crash

	const FName Key = Class->GetFName();
	UPool** const pPool = Pools.Find(Key);
	if (!pPool) {
		UE_LOG(LogJPool, Warning, TEXT("Pooler.GetPool. Could not find the pool for class=%s"), *Key.ToString());
		return nullptr;
	}

	return *pPool;
}

AActor* UPooler::Get(TSubclassOf<AActor> Class) {
	// TODO might be crashing here?
	UPool* const Pool = GetPool(Class); 
	if (!Pool) return nullptr;

	return Pool->Get();
}

void UPooler::Return(AActor* Actor) {
	if (!IsValid(Actor)) { // checking here to avoid problems on Actor->GetClass
		UE_LOG(LogJPool, Warning, TEXT("Pooler.Return. Actor was invalid, ignoring."));
		return;
	}

	UPool* const Pool = GetPool(Actor->GetClass());
	if (!Pool) {
		UE_LOG(LogJPool, Warning, TEXT("Pooler.Return. Could not find the pool. Destroying the actor."));
		Actor->Destroy();
		return;
	}

	Pool->Return(Actor);
}
