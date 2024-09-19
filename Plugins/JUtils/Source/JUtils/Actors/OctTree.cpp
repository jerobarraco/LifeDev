// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "OctTree.h"

#include "Pool.h"

DEFINE_LOG_CATEGORY_STATIC(LogJOctTree, Log, Log);

AOTNode::AOTNode() {
	Super::SetActorTickEnabled(false);
	PrimaryActorTick.SetTickFunctionEnable(false);
	PrimaryActorTick.bStartWithTickEnabled = false;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void AOTNode::AddActor(const AActor* const Actor) {}

void AOTNode::SetBounds(const FVector& InCornerA, const FVector& InCornerB) {
	// TODO error if it's already set
	CornerA = InCornerA;
	CornerB = InCornerB;
	if (Actors.Num()>0)
		UE_LOG(LogJOctTree, Warning, TEXT("%hs Rebounding with actors. lol."));
	// TODO implement reflow
}

void AOTNode::Reset() {
	Super::Reset();
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

	RootNode->AddActor(Actor);
}

void AOctTree::SetBounds(const FVector& CornerA, const FVector& CornerB) {
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("could not get the root"));
		return;
	}

	RootNode->SetBounds(CornerA, CornerB);
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
}

void AOctTree::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (RootNode) RootNode->Return();
	
	Pool->Empty();
	// Return all nodes
	Super::EndPlay(EndPlayReason);
}
