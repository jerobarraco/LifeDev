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
	bool IsInside(AActor* const Actor) const;

	// returns the owning node if any. also works as "contains"
	UFUNCTION(BlueprintCallable)
	AOTNode* Find(AActor* const Actor) const;

	// returns true when break
	UFUNCTION(BlueprintCallable)
	bool Iterate(const FJOTIterator& Iterator);
	
	// returns true when break
	UFUNCTION(BlueprintCallable)
	bool IterateInside(const FJOTIterator& Iterator, const FBox& Box);
	
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
	
	AOTNode* NodeForActor(AActor* const Actor);
	void Split();
	void SetBox(const FBox& InBox);
	void SetSubsBox();
	void AddToSub(AActor* const Actor);
	void PushToSubs();
	
	UPROPERTY(Transient)
	TArray<AOTNode*> Nodes; // children is already defined and has different meaning
	UPROPERTY(Transient)
	TArray<AActor*> Actors;
	UPROPERTY(Transient)
	FBox Box;
	UPROPERTY(Transient)
	class AOTNode* Parent = nullptr; // TODO

	uint8 ActorsMax = 1; // not optimized. TODO optimize this obnoxiously redundant variable (but it might be a feature) 
	friend class AOctTree;
};

// test. octree
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JUtils), meta=(BlueprintSpawnableComponent))
class JUTILS_API AOctTree: public AInfo { // an actor because of hunch
	GENERATED_BODY()

public:
	AOctTree();

	// TODO maybe remove this interface and just let them access the root.

	UFUNCTION(BlueprintCallable)
	void Add(AActor* const Actor);
	void operator+=(AActor* const Actor) {Add(Actor);};// because i can

	UFUNCTION(BlueprintCallable)
	void SetBox(const FBox& InBox);

	UFUNCTION(BlueprintCallable)
	void DbgDraw();
	
	UFUNCTION(BlueprintCallable)
	void Iterate(const FJOTIterator& Iterator) const;

	UFUNCTION(BlueprintCallable)
	void Print();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AOTNode* GetRoot() const { return RootNode; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	bool PrintIter(AActor* const A, AOTNode* const Node);
	
	UPROPERTY(Transient, BlueprintReadOnly)
	AOTNode* RootNode = nullptr;
	UPROPERTY(Transient) // cache
	UPool* Pool = nullptr;

	uint8 ActorsMax = 2;
};
