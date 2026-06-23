// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "Pool.generated.h"

// Note, i thought about the idea to make this into a object pool too. but there's some overhead.
// a couple of extra castings and isValid checks. and possibly another cast on the client code's side.
// at the moment i really don´t need a uobject pool. and can't really think of a case where that would be necessary.
// I'm using an actor since that's more likely the type of objects to get pooled and i can use the basic functions
// on them to set them up and disable. This is to avoid unnecessary interfaces and such. Whereas uobjects have none of that.
// so, K.I.S.S..

// basic actor pooling.
// you can use this directly if you want a local pool. but you would need to return the actors by a ref to your pool.
UCLASS(Blueprintable, BlueprintType, Category="JUtils|Pooler")
class JUTILS_API UPool: public UObject {
	GENERATED_BODY()

public:
	// sets the configuration for this pool. changing the class on a pool handled by Pooler can cause issues.
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler|Pool", meta=(AdvancedDisplay="SetTicks,CanGrow,TrimTime"))
	void Set(int32 const Max, const TSubclassOf<AActor> Class, bool const InSetTicks=true,
		bool const InCanGrow=false, int32 const InTrimTime=5);
	
	// gets an actor. returns null on exhausted or failure
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler|Pool")
	AActor* Get();

	// returns an actor to this pool.
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler|Pool")
	void Return(AActor* const Actor);

	////advanced: these are not really necessary outside this class. but can be helpful to have.
	
	// spwans a new actor and stores it in the ready list
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler|Pool", meta=(AdvancedDisplay))
	bool Spawn();
	// sets or reset the trim timer. calling it will stop the previous and wait again.
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler|Pool", meta=(AdvancedDisplay))
	void SetTrimTimer();
	// trims if possible, and schedule another timer if needed. (remove one and shrink up to the MAX)
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler|Pool", meta=(AdvancedDisplay))
	void Trim();
	// remove (destroy) one item. it will shrink. regardless of max. when 0 reached if canGrow is false the pool will stop working.
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler|Pool", meta=(AdvancedDisplay))
	bool Remove();
	// empties the pool. Destroys the objects that are NOT used.
	// when those objects are returned to the Pooler they will get destroyed if this pool doesn't exist anymore.
	// if canGrow is false, then this pool won't work anymore unless calling Set.
	// if Set is called with a different class then empty will be called first.
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler|Pool", meta=(AdvancedDisplay))
	void Empty();
	/// ~advanced

	virtual void BeginDestroy() override;

	// manage ticks on actors
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool SetTicks = true;

	// allows this pool to add items when exhausted
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool CanGrow = false;

	// if > 0, allow the pool to shrink over time.
	// this is the time between each trim, at which one item will be removed while keeping at least the MAX set.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	int32 TrimTime = 0;

protected:
	int32 ItemMax = 0;
	TSubclassOf<AActor> ItemType;
	FTimerHandle TrimTimer;
	
	UPROPERTY(Transient) // gcd
	TArray<TObjectPtr<AActor>> Ready;
};

// basic actor pooling. handles multiple pools based on the class to spawn
UCLASS(Blueprintable, Category="JUtils|Pooler")
class JUTILS_API UPooler: public UWorldSubsystem {
	GENERATED_BODY()

public:
	static UPooler* Instance(UObject* const Ctx);
	
	// adds a new pool for a class. or modifies an already existing one. returns it.
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler")
	UPool* SetPool(int32 const Max, TSubclassOf<AActor> const Class,
		bool const SetTicks=true, bool const CanGrow=false, int32 const TrimTime = 5);

	// removes (deletes) a pool for a given class. objects that are unused will be destroyed. the rest will be destroyed on return. 
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler")
	void RemPool(TSubclassOf<AActor> const Class);

	// gets a managed pool. don't call Set on that pool with a different class,
	// or you'll have problems.
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler")
	UPool* GetPool(const TSubclassOf<AActor> Class);

	// gets an actor of a class from a pool. Returns null if exhausted or the pool has not been created.
	// if you're going to use this often, is better to get the pool, and call get there.
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler")
	AActor* Get(const TSubclassOf<AActor> Class);

	// returns an actor to the corresponding pool.
	// If the pool is removed, then the actor will get destroyed.
	UFUNCTION(BlueprintCallable, Category="JUtils|Pooler")
	void Return(AActor* const Actor);

protected:
	UPROPERTY(Transient) // gcd
	TMap<FName, TObjectPtr<UPool>> Pools;
};
