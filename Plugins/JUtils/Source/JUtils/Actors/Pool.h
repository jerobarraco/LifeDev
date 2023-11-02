#pragma once

#include "Pool.generated.h"

// basic actor pooling. manages one local pool.
UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UPool: public UObject {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void Set(int32 Max, bool bCanIncrease, TSubclassOf<AActor> Class);
    UFUNCTION(BlueprintCallable)
    AActor* Get();
    UFUNCTION(BlueprintCallable)
    void Return(AActor* A);
    UFUNCTION(BlueprintCallable)
    void Empty();

    virtual void BeginDestroy() override;
protected:
    
    UPROPERTY()
    TArray<AActor*> Ready;
    friend class UPooler;
};

// basic actor pooling. handles multiple pools
UCLASS(Blueprintable)
class JUTILS_API UPooler: public UWorldSubsystem {
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable)
    void AddPool(int32 Max, bool CanIncrease, TSubclassOf<AActor> Class);

    UFUNCTION(BlueprintCallable)
    void RemPool(TSubclassOf<AActor> Class);

    UFUNCTION(BlueprintCallable)
    UPool* GetPool(TSubclassOf<AActor> Class);

    UFUNCTION(BlueprintCallable)
    AActor* Get(TSubclassOf<AActor> Class);

    UFUNCTION(BlueprintCallable)
    void Return(AActor* A);
    
protected:
    UPROPERTY() // gcd
    TMap<FName, UPool*> Pools;
};
