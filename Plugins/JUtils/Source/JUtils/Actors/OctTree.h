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
	void Add(const AActor* const Actor);
	void AddToSub(const AActor* const Actor);
	void SetBounds(const FVector& InCornerA, const FVector& InCornerB);
	void PushToSubs();
	void Split();
	
	virtual void Reset() override;
	void Empty();
	void Return();

	// biology is the only subject in which multiply and divide is the same. // smoke test
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(Transient)
	TArray<AOTNode*> Subs; // children is already defined and has different meaning
	UPROPERTY(Transient)
	TArray<const AActor*> Actors;
	UPROPERTY(Transient)
	FVector CornerA;
	UPROPERTY(Transient)
	FVector CornerB;

	uint8 ActorsMax = 5; // not optimized. TODO optimize this obnoxiously redundant variable (but it might be a feature) 
	friend class AOctTree;
};

// test. octree
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JUtils), meta=(BlueprintSpawnableComponent))
class JUTILS_API AOctTree: public AInfo { // an actor because of hunch
	GENERATED_BODY()

public:
	AOctTree();

	void AddActor(const AActor* const Actor);
	void SetBounds(const FVector& CornerA, const FVector& CornerB);

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
