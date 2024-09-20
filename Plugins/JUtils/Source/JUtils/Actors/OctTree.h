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
	bool Add(AActor* const Actor);
	void operator+=(AActor* const Actor) {Add(Actor);} // because i can

	// not recursive. use contains to find the container
	UFUNCTION(BlueprintCallable)
	int32 Rem(AActor* const Actor);
	void operator-=(AActor* const Actor) {Rem(Actor);} // because i can

	UFUNCTION(BlueprintCallable)
	bool IsInside(AActor* const Actor) const;

	// returns the owning node if any. also works as "find". does not check positions.
	UFUNCTION(BlueprintCallable)
	AOTNode* Contains(AActor* const Actor) const;

	// returns true when break
	UFUNCTION(BlueprintCallable)
	bool Iterate(const FJOTIterator& Iterator);
	// returns true when you want to break. iterate through actors inside a box.
	UFUNCTION(BlueprintCallable)
	bool IterateInside(const FJOTIterator& Iterator, const FBox& Box);
	UFUNCTION(BlueprintCallable, CallInEditor, meta=(AdvancedDisplay))
	void Pack();
	UFUNCTION(BlueprintCallable, CallInEditor, meta=(AdvancedDisplay))
	void DbgDraw();

	// biology is the only subject in which multiply and divide is the same. // smoke test
protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;
	
	void SetUp(const int32 ActorsMax = 10);
	AOTNode* NodeForActor(AActor* const Actor);
	AOTNode* ClosestNode(AActor* Actor);
	void Split();
	void SetBox(const FBox& InBox);
	void SetSubsBox();
	bool AddToSub(AActor* const Actor);
	void PushToSubs();
	void Empty(const bool ReturnSubs = true);
	void Return(const bool ReturnSubs = true);

	UPROPERTY(Transient)
	TArray<AOTNode*> Nodes; // children is already defined and has different meaning
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
	void operator+=(AActor* const Actor) {Add(Actor);};// because i can

	// removes from the tree. recursive
	UFUNCTION(BlueprintCallable)
	int32 Rem(AActor* const Actor);
	void operator-=(AActor* const Actor) {Rem(Actor);};// because i can

	// updates an actor, modifying the tree, this could be slower than just rebuilding depending on how many actors move.
	UFUNCTION(BlueprintCallable)
	bool Update(AActor* const Actor);

	UFUNCTION(BlueprintCallable)
	void SetBox(const FBox& InBox);

	// TODO setup calls rebuild and sets max
	UFUNCTION(BlueprintCallable)
	void SetUp(const FBox& InBox, const int32 Max) {};

	UFUNCTION(BlueprintCallable)
	void Iterate(const FJOTIterator& Iterator) const;

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(AdvancedDisplay))
	void DbgDraw();
	UFUNCTION(BlueprintCallable, CallInEditor, meta=(AdvancedDisplay))
	void Pack();
	
	UFUNCTION(BlueprintCallable, CallInEditor, CallInEditor)
	void Print();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AOTNode* GetRoot() const { return RootNode; }

	UFUNCTION(BlueprintCallable, CallInEditor)
	void RebuildSameBox(); // TODO rename
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Rebuild(const FBox& NewBox);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION() // bound
	bool PrintIter(AActor* const A, AOTNode* const Node);
	// true if succeeded or ok. false otherwise.
	bool TryExtend(AActor* Actor);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ExtendMax = 5;

	UPROPERTY(Transient, BlueprintReadOnly)
	AOTNode* RootNode = nullptr;
	UPROPERTY(Transient) // cache
	UPool* Pool = nullptr;

	uint8 ActorsMax = 2; // needs to be elsewhere so thta i can set the max
};
