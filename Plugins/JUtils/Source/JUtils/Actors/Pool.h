#pragma once

#include "Pool.generated.h"

// basic actor pooling.
// you can use this directly if you want a local pool. but you would need to return the actors by a ref to your pool.
UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UPool: public UObject {
    GENERATED_BODY()

public:
    // sets the configuration for this pool. changing the class on a pool handled by Pooler can cause issues.
    UFUNCTION(BlueprintCallable)
    void Set(int32 Max, TSubclassOf<AActor> Class, bool SetTicks = true, bool bCanGrow=false, int32 InTrimTime = 5);
    
    // gets an actor. returns null on exhausted or failure
    UFUNCTION(BlueprintCallable)
    AActor* Get();
    // returns an actor to this pool.
    UFUNCTION(BlueprintCallable)
    void Return(AActor* Actor);

    ////advanced: these are not really necessary outside this class. but can be helpful to have.
    
    // spwans a new actor and stores it in the ready list
    UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
    bool Spawn();
    // sets or reset the timer. calling it will stop the previous and wait again.
    UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
    void SetTrimTimer();
    // trims if possible, and schedule another timer if needed. (remove one and shrink up to the MAX)
    UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
    void Trim();
    // remove one item. it will shrink. regardless of max. when 0 reached if canGrow is false the pool will stop working.
    UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
    bool RemoveOne();
    // empties the pool. if canGrow is false, then this pool won't work anymore unless calling Set.
    // if Set is called with a different class then empty will be called first
    UFUNCTION(BlueprintCallable, meta=(AdvancedDisplay))
    void Empty();
    /// ~advanced
    
    virtual void BeginDestroy() override;

    // manage ticks on actors
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
    bool SetTicks = true;

    // allows this pool to add items when exhausted
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
    bool CanGrow = false;

    // allow the pool to shrink over time. one every this much time.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
    int32 TrimTime = 0;

protected:
    int32 ItemMax = 0;
    TSubclassOf<AActor> ItemType;
    FTimerHandle TrimTimer;
    
    UPROPERTY(Transient) // gcd
    TArray<AActor*> Ready;
};

// basic actor pooling. handles multiple pools based on the class to spawn
UCLASS(Blueprintable)
class JUTILS_API UPooler: public UWorldSubsystem {
    GENERATED_BODY()
public:

    // adds a new pool for a class. or modifies an already existing one.
    UFUNCTION(BlueprintCallable)
    bool AddPool(int32 Max, TSubclassOf<AActor> Class, bool SetTicks=true, bool CanGrow=false, int32 TrimTime = 5);

    // removes (deletes) a pool for a given class.
    UFUNCTION(BlueprintCallable)
    void RemPool(TSubclassOf<AActor> Class);

    // gets a managed pool. don't call set on that pool with another class or youll have problems.
    UFUNCTION(BlueprintCallable)
    UPool* GetPool(TSubclassOf<AActor> Class);

    // gets an actor of a class from a pool
    UFUNCTION(BlueprintCallable)
    AActor* Get(TSubclassOf<AActor> Class);

    // returns an actor to the corresponding pool
    UFUNCTION(BlueprintCallable)
    void Return(AActor* Actor);
    
protected:
    UPROPERTY(Transient) // gcd
    TMap<FName, UPool*> Pools;
};
