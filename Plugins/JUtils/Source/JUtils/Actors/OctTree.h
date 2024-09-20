// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "OctTree.generated.h"

class UPool;
class AOTNode;

DECLARE_DYNAMIC_DELEGATE_RetVal_TwoParams(bool, FJOTIterator, AActor* const, Actor, AOTNode* const, Node);

UCLASS(Blueprintable)
class JUTILS_API AOTNode: public AInfo { // an actor so that it can be pooled.
	GENERATED_BODY()
public:
	AOTNode();
	UFUNCTION(BlueprintCallable)
	void Add(AActor* const Actor, AOTNode* NotTo=nullptr);
	void operator+=(AActor* const Actor) {Add(Actor);};// because i can


	UFUNCTION(BlueprintCallable)
	void Split();
	UFUNCTION(BlueprintCallable)
	AOTNode* SubForActor(AActor* const Actor);
	UFUNCTION(BlueprintCallable)
	bool IsInside(AActor* const Actor) const;

	// returns the owning node if any
	UFUNCTION(BlueprintCallable)
	AOTNode* Find(AActor* const Actor) const;

	// returns true when break
	UFUNCTION(BlueprintCallable)
	bool Iterate(const FJOTIterator& Iterator);
	
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
	void SetSubsBox();
	void AddToSub(AActor* const Actor);
	void PushToSubs();
	
	UPROPERTY(Transient)
	TArray<AOTNode*> Subs; // children is already defined and has different meaning
	UPROPERTY(Transient)
	TArray<AActor*> Actors;
	UPROPERTY(Transient)
	FBox Box;

	uint8 ActorsMax = 1; // not optimized. TODO optimize this obnoxiously redundant variable (but it might be a feature) 
	friend class AOctTree;
};

// test. octree
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JUtils), meta=(BlueprintSpawnableComponent))
class JUTILS_API AOctTree: public AInfo { // an actor because of hunch
	GENERATED_BODY()

public:
	AOctTree();

	UFUNCTION(BlueprintCallable)
	void Add(AActor* const Actor);

	UFUNCTION(BlueprintCallable)
	void SetBox(const FBox& InBox);

	UFUNCTION(BlueprintCallable, BlueprintCallable)
	void DbgDraw();
	
	UFUNCTION(BlueprintCallable)
	void Iterate(const FJOTIterator& Iterator) const;

	UFUNCTION(BlueprintCallable, BlueprintCallable)
	void Print();

	void operator+=(AActor* const Actor) {Add(Actor);};// because i can

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	bool PrintIter(AActor* const A, AOTNode* const Node);
	
	UPROPERTY(Transient)
	AOTNode* RootNode = nullptr;
	UPROPERTY(Transient)
	UPool* Pool = nullptr;

	uint8 ActorsMax = 2;
};
