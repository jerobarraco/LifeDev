#include "Pool.h"

DEFINE_LOG_CATEGORY_STATIC(LogJPool, Log, Log);

#pragma optimize("", off)
void UPool::Set(int32 Max, bool bCanIncrease, TSubclassOf<AActor> Class) {
    Empty();
    
    static FActorSpawnParameters P;
    P.bNoFail = true;
    P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Ready.Reserve(Max);
    if (Ready.Num()>Max) {
        // TODO destroy
    }
    // TODO CanIncrease
    // TODO store class to increase
    if (Ready.Num()<Max) {
        for (int32 i = Ready.Num(); i<Max; ++i) {
            AActor* const Actor = GetWorld()->SpawnActor(Class, 0, 0, P);
            if (!IsValid(Actor)) {
                UE_LOG(LogJPool, Log, TEXT("Could not spawn the actor"));
                continue;
            }
            Ready.Add(Actor);
            Actor->SetActorHiddenInGame(true);
        }
    }
}

AActor* UPool::Get() {
    if (Ready.Num()<=0) {
        UE_LOG(LogJPool, Log, TEXT("Pool is exhausted so can't return an actor."));
        return nullptr;
    }
    
    AActor* const A = Ready[0];
    Ready.RemoveAt(0, 1, false);
    A->SetActorHiddenInGame(false);
    A->Reset();
    UE_LOG(LogJPool, Log, TEXT("Pool gave an actor."));
    return A;
}

void UPool::Return(AActor* A) {
    UE_LOG(LogJPool, Log, TEXT("Actor returned to pool."));
    A->SetActorHiddenInGame(true);
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

void UPooler::AddPool(int32 Max, bool CanIncrease, TSubclassOf<AActor> Class) {
    const FName Key = Class->GetFName();
    UPool** pPool = Pools.Find(Key);
    UPool* Pool = nullptr;
    if (pPool) {
        Pool = *pPool;
    }else{
        Pool = NewObject<UPool>(this);
        Pools.Add(Key, Pool);
    }

    Pool->Set(Max, CanIncrease, Class);
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
        UE_LOG(LogJPool, Log, TEXT("Pooler.Return. Could not find the pool for class=%s. Destroying the actor."), *Key.ToString());
        A->Destroy();
        return;
    }

    Pool->Return(A);
}

#pragma optimize("", on)
