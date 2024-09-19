// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "OctTree.h"

#include "Pool.h"

DEFINE_LOG_CATEGORY_STATIC(LogJOctTree, Log, Log);

// im pulling the algo out of my ... hat.

AOTNode::AOTNode() {
	Super::SetActorTickEnabled(false);
	PrimaryActorTick.SetTickFunctionEnable(false);
	PrimaryActorTick.bStartWithTickEnabled = false;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void AOTNode::Add(const AActor* const Actor) {
	// TODO test bounds and reject the rejected
	// Dont store the actors in this instance if it's split already. wasting a tarray.
	if (Subs.Num()==0) {
		if (Actors.Num()<ActorsMax) {
			Actors.Add(Actor);
			return;
		} else
			Split();
	}
	AddToSub(Actor);
}

void AOTNode::AddToSub(const AActor* const Actor) {
	AOTNode* const S = SubForActor(Actor);
	if (!S) return; // already logged
	S->Add(Actor); // will trickle down and split. "recursively" (though different objects)
}

void AOTNode::SetBox(const FBox& InBox) {
	if (Actors.Num()>0)
		UE_LOG(LogJOctTree, Warning, TEXT("%hs Rebounding with actors. lol."), __func__);
	// why bother. this is not meant to be optimal yet
	Box = InBox;
	// TODO error if it's already set
	// TODO implement reflow.
}

AOTNode* AOTNode::SubForActor(const AActor* const Actor) {
	for (AOTNode* const S: Subs) {
		if (IsValid(S) && S->Contains(Actor)) return S;
	}

	UE_LOG(LogJOctTree, Warning, TEXT("%hs Could not find it"), __func__);
	return nullptr;
}

bool AOTNode::Contains(const AActor* const Actor) const {
	if (!IsValid(Actor)) return false; // seems too little for a func, but im sure ill use it later on.
	const FVector& AT = Actor->GetActorLocation();
	return Box.IsInsideOrOn(AT);
}

void AOTNode::PushToSubs() {
	// 2nd move the actors to subs
	for (const AActor* A: Actors) {
		AddToSub(A);
	}
	Actors.Empty();
}

void AOTNode::Split() {
	constexpr uint8 SubsNum = 8;
	UPooler* const Pooler = UPooler::Instance(this);
	UPool* const Pool = Pooler? Pooler->GetPool(AOTNode::StaticClass()) : nullptr;
	if (!Pooler || !Pool) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs can't"), __func__);
		return;
	}
	
	if (Subs.Num() < SubsNum) {
		// WTF DEGENERATE CASE! but meh
		// 1st create subs
		while (Subs.Num()<SubsNum) {
			AOTNode* const S = Cast<AOTNode>(Pool->Get());
			if (!S) {
				UE_LOG(LogJOctTree, Warning, TEXT("%hs can't 2 "), __func__);
				return;
			}
			S->ActorsMax = ActorsMax;
			Subs.Add(S);
		}
	}

	PushToSubs();
}

void AOTNode::Reset() {
	Super::Reset();
	// Empty(); // should be empty from the return. 
}

void AOTNode::Empty() {
	for (AOTNode* const S:Subs) {
		if (!S) continue;
		S->Return();
	}
	Subs.Empty();
	Actors.Empty();
}

void AOTNode::Return() {
	UPooler* const Pooler = UPooler::Instance(this);
	if (!Pooler) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs cant"), __func__);
		return;
	}

	Empty();
	Pooler->Return(this);
}

void AOTNode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Empty();
	Super::EndPlay(EndPlayReason);
}

AOctTree::AOctTree(): Super() {
	Super::SetActorTickEnabled(false);
	PrimaryActorTick.SetTickFunctionEnable(false);
	PrimaryActorTick.bStartWithTickEnabled = false;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void AOctTree::AddActor(const AActor* const Actor) {
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("could not get the root"));
		return;
	}

	RootNode->Add(Actor);
}

void AOctTree::SetBox(const FBox& InBox) {
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("could not get the root"));
		return;
	}

	RootNode->SetBox(InBox);
}

void AOctTree::BeginPlay() {
	Super::BeginPlay();

	UPooler* const Pooler = UPooler::Instance(this);
	if (!Pooler) {
		UE_LOG(LogJOctTree, Warning, TEXT("Could not obtain the Pooler. this would crash later."));
		return;
	}

	Pool = Pooler->SetPool(1, AOTNode::StaticClass(), false, true, 10);
	RootNode = Cast<AOTNode>(Pool->Get());
	if (!RootNode) return;
	RootNode->ActorsMax = ActorsMax;
}

void AOctTree::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (RootNode) RootNode->Return();
	
	Pool->Empty();
	// Return all nodes
	Super::EndPlay(EndPlayReason);
}
