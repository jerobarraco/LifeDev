#pragma once

#include "Pool.generated.h"

// basic actor pooling. manages one local pool.
UCLASS(Blueprintable, BlueprintType)
class JUTILS_API UPool: public UObject {
    GENERATED_BODY()

public:
    bool Spawn();
    UFUNCTION(BlueprintCallable)
    void Set(int32 Max, TSubclassOf<AActor> Class, bool SetTicks = true, bool bCanGrow=false);
    UFUNCTION(BlueprintCallable)
    AActor* Get();
    UFUNCTION(BlueprintCallable)
    void Return(AActor* A);
    UFUNCTION(BlueprintCallable)
    void Empty();

    virtual void BeginDestroy() override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
    bool SetTicks = true;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
    bool CanGrow = false;

protected:
    TSubclassOf<AActor> ChildType;
    
    UPROPERTY() // gcd
    TArray<AActor*> Ready;
};

// basic actor pooling. handles multiple pools
UCLASS(Blueprintable)
class JUTILS_API UPooler: public UWorldSubsystem {
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable)
    void AddPool(int32 Max, TSubclassOf<AActor> Class, bool SetTicks=true, bool CanGrow=false);

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
