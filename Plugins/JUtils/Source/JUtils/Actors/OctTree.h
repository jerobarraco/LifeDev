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

	// returns true if the Actor is inside the bounds of this node. 
	UFUNCTION(BlueprintCallable)
	bool IsInside(AActor* const Actor) const;

	// returns the owning node if any. also works as "find". does not check positions.
	UFUNCTION(BlueprintCallable)
	AOTNode* Contains(AActor* const Actor) const;

	// iterate through actors
	// returns true when break
	UFUNCTION(BlueprintCallable)
	bool Iterate(const FJOTIterator& Iterator);
	// iterate through actors inside a box.
	// returns true when you want to break.
	UFUNCTION(BlueprintCallable)
	bool IterateIn(const FJOTIterator& Iterator, const FBox& InBox);

	UFUNCTION(BlueprintCallable, CallInEditor)
	TArray<AOTNode*> GetNodes() { return Nodes; }

	UFUNCTION(BlueprintCallable, CallInEditor)
	TArray<AActor*> GetActors() { return Actors; }
	
	// recursive
	UFUNCTION(BlueprintCallable, CallInEditor, meta=(AdvancedDisplay))
	void Pack();
	UFUNCTION(BlueprintCallable, CallInEditor, meta=(AdvancedDisplay, AutoCreateRefTerm="BoxColor, ActorColor"))
	void DbgDraw(const FColor& BoxColor, const FColor& ActorColor = FColor::Yellow);

	UFUNCTION(BlueprintCallable)
	FString ToString() const;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;

	/// all this functions can't be safely exposed to bp or something bad would happen
	void SetActorsMax(const int32 InActorsMax = 10);
	void SetBox(const FBox& InBox);
	// biology is the only subject in which multiply and divide is the same. // smoke test
	void Split();
	AOTNode* ClosestNode(const FVector& To);
	void SetNodesBox();
	bool AddToNodes(AActor* const Actor);
	// move the actors to sub nodes
	void PushToNodes();
	void Empty(const bool ReturnSubs = true);
	void Return(const bool ReturnSubs = true);

	UPROPERTY(Transient)
	TArray<AOTNode*> Nodes; // "children" is already defined, and has different meaning.
	UPROPERTY(Transient)
	TArray<AActor*> Actors;
	
	UPROPERTY(Transient)
	FBox Box;

	uint8 ActorsMax = 1;
	// not optimized. TODO optimize this obnoxiously redundant variable (but it might be a feature)
	// can't make static since i want multiple trees with different maxes.

	friend class AOctTree;
};

// octree. Call the Set* functions before calling the other functions.
UCLASS(Blueprintable, BlueprintType, ClassGroup=(JUtils), meta=(BlueprintSpawnableComponent))
class JUTILS_API AOctTree: public AInfo { // an actor because of hunch
	GENERATED_BODY()

public:
	AOctTree();

	UFUNCTION(BlueprintCallable)
	void Add(AActor* const Actor);
	void operator+=(AActor* const Actor) {Add(Actor);} // because i can

	// removes from the tree. recursive
	UFUNCTION(BlueprintCallable)
	int32 Rem(AActor* const Actor);
	void operator-=(AActor* const Actor) {Rem(Actor);} // because i can

	// updates an actor, modifying the tree, this could be slower than just rebuilding depending on how many actors move.
	// doesn't call pack, so you can update many objects before packing. or you might wanna pack not every update.
	UFUNCTION(BlueprintCallable)
	bool Update(AActor* const Actor);

	// will try to compact the tree and remove un-needed nodes. it won't change the max bounds (or root)
	// Slow and delicate.
	// call after updating all the objects. you can also do it less often.
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Pack();

	// call this at the very start. or the tree won't work.
	// warning. will rebuild the tree
	UFUNCTION(BlueprintCallable)
	void SetBox(const FBox& InBox);
	UFUNCTION(BlueprintCallable)
	void SetActorsMax(int32 InActorsMax);
	UFUNCTION(BlueprintCallable)
	void SetPoolTrimTime(float const InTrimTime);

	// iterate through actors
	// returns true when break
	UFUNCTION(BlueprintCallable)
	void Iterate(const FJOTIterator& Iterator); // if i make it const. then ue makes it pure. :(

	// iterate through actors
	// returns true when break
	UFUNCTION(BlueprintCallable)
	void IterateIn(const FJOTIterator& Iterator, const FBox& Box);

	// will rebuild the tree. slower than 1 update. but faster if many objects moved.
	// no need to pack. it will be optimal.
	UFUNCTION(BlueprintCallable, CallInEditor)
	void Rebuild();

	UFUNCTION(BlueprintCallable, CallInEditor, meta=(AdvancedDisplay, AutoCreateRefTerm="BoxColor, ActorColor"))
	void DbgDraw(const FColor& BoxColor = FColor::Blue, const FColor& ActorColor = FColor::Yellow);
	UFUNCTION(BlueprintCallable, CallInEditor, meta=(AdvancedDisplay))
	void Print();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AOTNode* GetRoot() const { return RootNode; }
	// Be careful. this will destroy the pool for ALL oct trees. if there's one alive it might fail.
	// but after destroying the pool you can just create a new oct tree, and it'll create a new pool.
	UFUNCTION(BlueprintCallable, CallInEditor)
	void DestroyPool();

	// when trying to extend, what's the max levels it will try to go (each try)
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ExtendMax = 5;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION() // bound
	bool PrintIter(AActor* const A, AOTNode* const Node);
	// true if succeeded or ok. false otherwise.
	bool TryExtend(AActor* Actor);

	UPROPERTY(BlueprintReadOnly)
	int32 ActorsMax = 5;

	UPROPERTY(Transient, BlueprintReadOnly)
	AOTNode* RootNode = nullptr;
	UPROPERTY(Transient, BlueprintReadOnly) // cache
	UPool* Pool = nullptr;
};
