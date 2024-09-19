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

void AOTNode::Reset() {
	Super::Reset();
}

void AOTNode::Return() {
	UPooler* const Pooler = UPooler::Instance(this);
	if (!Pooler) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs cant"), __func__);
		return;
	}

	for (AOTNode* const S:Subs) {
		if (!S) continue;
		S->Return();
	}
	Subs.Empty();
	Pooler->Return(this);
}

AOctTree::AOctTree(): Super() {
	Super::SetActorTickEnabled(false);
	PrimaryActorTick.SetTickFunctionEnable(false);
	PrimaryActorTick.bStartWithTickEnabled = false;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void AOctTree::AddActor(const AActor* const Actor) {}

void AOctTree::BeginPlay() {
	Super::BeginPlay();

	UPooler* const Pooler = UPooler::Instance(this);
	if (!Pooler) {
		UE_LOG(LogTemp, Warning, TEXT("Could not obtain the Pooler. this would crash later."));
		return;
	}

	Pool = Pooler->SetPool(1, AOTNode::StaticClass(), false, true, 10);
}

void AOctTree::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (RootNode) RootNode->Return();
	
	Pool->Empty();
	// Return all nodes
	Super::EndPlay(EndPlayReason);
}
