#include "Pool.h"

DEFINE_LOG_CATEGORY_STATIC(LogJPool, Log, Log);

bool UPool::Spawn() {
        
    static FActorSpawnParameters P;
    P.bNoFail = true;
    P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* const Actor = GetWorld()->SpawnActor(ChildType, 0, 0, P);
    if (!IsValid(Actor)) {
        UE_LOG(LogJPool, Log, TEXT("Could not spawn the actor."));
        return false;
    }

    Ready.Add(Actor);
    Actor->SetActorHiddenInGame(true);
    return true;
}

void UPool::Set(int32 Max, TSubclassOf<AActor> Class, bool InSetTicks, bool InCanGrow) {
    Empty();
    
    SetTicks = InSetTicks;
    CanGrow = InCanGrow;
    ChildType = Class;

    while (Ready.Num()>Max) {
        Ready[0]->Destroy();
        Ready.RemoveAtSwap(0, 1, true);
    }
    
    Ready.Reserve(Max);
    while (Ready.Num()<Max-1) {
        if (!Spawn()) break; // avoid infinite loops
    }
}


AActor* UPool::Get() {
    if (Ready.Num()<=0) {
        if (!CanGrow) {
            UE_LOG(LogJPool, Log, TEXT("Pool is exhausted, and can't grow. so can't return an actor."));
            return nullptr;
        }
        if (!Spawn()) return nullptr;
    }
    
    AActor* const A = Ready[0];
    Ready.RemoveAt(0, 1, false);
    A->SetActorHiddenInGame(false);
    A->Reset();
    if (SetTicks) {
        A->SetActorTickEnabled(true);
    }
    
    UE_LOG(LogJPool, Log, TEXT("Pool gave an actor."));
    return A;
}

void UPool::Return(AActor* A) {
    UE_LOG(LogJPool, Log, TEXT("Actor returned to pool."));
    if (!IsValid(A)) {
        UE_LOG(LogJPool, Log, TEXT("Return: Actor was invalid."));
        return;
    }

    A->SetActorHiddenInGame(true);
    if (SetTicks) {
        A->SetActorTickEnabled(false);
    }
    Ready.AddUnique(A);
}

void UPool::Empty() {
    UE_LOG(LogJPool, Log, TEXT("Clearing pool"));
    for(AActor* const A: Ready) {
        A->Destroy();
    }
    Ready.Empty();
}

void UPool::BeginDestroy() {
    Empty();
    UObject::BeginDestroy();
}

void UPooler::AddPool(int32 Max, TSubclassOf<AActor> Class, bool SetTicks, bool CanGrow) {
    const FName Key = Class->GetFName();
    UPool** pPool = Pools.Find(Key);
    UPool* Pool = nullptr;
    if (pPool) {
        Pool = *pPool;
    }else{
        Pool = NewObject<UPool>(this);
        Pools.Add(Key, Pool);
    }

    Pool->Set(Max, Class, SetTicks, CanGrow);
}

void UPooler::RemPool(TSubclassOf<AActor> Class) {
    const FName Key = Class->GetFName();
    
    UPool** const PPool = Pools.Find(Key);
    if (!PPool) {
        UE_LOG(LogJPool, Log, TEXT("DelPool. Could not find the pool"));
        return;
    }

    UPool* const Pool = *PPool; 
    Pool->Empty();
    
    Pools.Remove(Key);
}

UPool* UPooler::GetPool(TSubclassOf<AActor> Class) {
    const FName Key = Class->GetFName();
    UPool** const pPool = Pools.Find(Key);
    if (!pPool) {
        UE_LOG(LogJPool, Log, TEXT("Pooler.GetPool. Could not find the pool for class=%s"), *Key.ToString());
        return nullptr;
    }
    return *pPool;
}

AActor* UPooler::Get(TSubclassOf<AActor> Class) {
    UPool* const Pool = GetPool(Class); 
    if (!Pool) {
        return nullptr;
    }
    return Pool->Get();
}

void UPooler::Return(AActor* A) {
    if (!IsValid(A)) {
        UE_LOG(LogJPool, Log, TEXT("Pooler.Return. Actor was invalid, ignoring."));
        return;
    }

    UPool* const Pool = GetPool(A->GetClass());
    if (!Pool) {
        UE_LOG(LogJPool, Log, TEXT("Pooler.Return. Could not find the pool. Destroying the actor."));
        A->Destroy();
        return;
    }

    Pool->Return(A);
}
