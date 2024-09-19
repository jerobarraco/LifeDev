// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "OctTree.generated.h"

class UPool;

UCLASS(Blueprintable)
class JUTILS_API AOTNode: public AInfo { // an actor so that it can be pooled.
	GENERATED_BODY()
public:
	AOTNode();
	UFUNCTION(BlueprintCallable)
	void Add(const AActor* const Actor);
	void operator+=(const AActor* const Actor) {Add(Actor);};// because i can

	UFUNCTION(BlueprintCallable)
	void AddToSub(const AActor* const Actor);
	UFUNCTION(BlueprintCallable)
	void PushToSubs();
	UFUNCTION(BlueprintCallable)
	void Split();
	UFUNCTION(BlueprintCallable)
	AOTNode* SubForActor(const AActor* const Actor);
	UFUNCTION(BlueprintCallable)
	bool Contains(const AActor* const Actor) const;
	
	virtual void Reset() override;
	UFUNCTION(BlueprintCallable)
	void Empty();
	UFUNCTION(BlueprintCallable)
	void Return();
	UFUNCTION(BlueprintCallable)
	void DbgDraw();

	// biology is the only subject in which multiply and divide is the same. // smoke test
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void SetBox(const FBox& InBox);

	UPROPERTY(Transient)
	TArray<AOTNode*> Subs; // children is already defined and has different meaning
	UPROPERTY(Transient)
	TArray<const AActor*> Actors;
	UPROPERTY(Transient)
	FBox Box;

	uint8 ActorsMax = 5; // not optimized. TODO optimize this obnoxiously redundant variable (but it might be a feature) 
	friend class AOctTree;
};

// test. octree
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JUtils), meta=(BlueprintSpawnableComponent))
class JUTILS_API AOctTree: public AInfo { // an actor because of hunch
	GENERATED_BODY()

public:
	AOctTree();

	UFUNCTION(BlueprintCallable)
	void Add(const AActor* const Actor);

	UFUNCTION(BlueprintCallable)
	void SetBox(const FBox& InBox);

	UFUNCTION(BlueprintCallable)
	void DbgDraw();
	
	void operator+=(const AActor* const Actor) {Add(Actor);};// because i can

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(Transient)
	AOTNode* RootNode = nullptr;
	UPROPERTY(Transient)
	UPool* Pool = nullptr;

	FVector Center;
	uint8 ActorsMax = 4;
};
