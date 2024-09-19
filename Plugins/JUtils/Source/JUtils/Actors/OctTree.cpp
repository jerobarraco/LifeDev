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

void AOTNode::SetSubsBox() {
	const int32 Num = Subs.Num();

	FVector C, E, HE, Max, HE2;
	Box.GetCenterAndExtents(C, E);
	HE = E/2;
	// surely ill need it to update the bounds if i ever do implement that
	for (uint8 i=0; i<Num; ++i) {
		AOTNode* const S = Subs[i];
		if (!IsValid(S)) continue;
		// maybe there's an optimal way to do this. sorry
		
		// im gonna use min as C for all of them so what? i hope ue will normalize my lame-ness. whats min and max in 3d anyway?
		// im sure itll bite me
		HE2 = HE;
		if (i==0) {// will it blendL??
		} else if (i==1) {
			HE2.X = -HE2.X;
		} else if (i==2) {
			HE2.Y = -HE2.Y;
		} else if (i==3) {
			HE2.Z = -HE2.Z;
		} else if (i==4) {
			HE2.X = -HE2.X; // this can be optimized but i dont feel like now. apollo-gies
			HE2.Y = -HE2.Y; // this can be optimized but i dont feel like now. apollo-gies
		} else if (i==5) {
			HE2.X = -HE2.X;
			HE2.Z = -HE2.Z;
		} else if (i==6) {
			HE2.Y = -HE2.Y;
			HE2.Z = -HE2.Z;
		} else if (i==7) {
			HE2.X = -HE2.X;
			HE2.Y = -HE2.Y;
			HE2.Z = -HE2.Z;
		}
		// without thinking it too much. it fits....
		Max = C+HE2;
		S->SetBox(FBox(C, Max)); // "let's start from the top"
			// ez
	}
	// let's assume we have what we need

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
		// what about the bounds buddy???????
		SetSubsBox();
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

void AOTNode::DbgDraw() {
	FVector C, E;
	Box.GetCenterAndExtents(C, E);
	DrawDebugBox(GetWorld(), C, E, FColor::Purple, false, 1, 0, 3);
	for (const AActor* const A: Actors) {
		if(!IsValid(A)) continue;
		DrawDebugPoint(GetWorld(), A->GetActorLocation(), 4, FColor::Orange, false, 1, 0);
	}

	for (AOTNode* const S: Subs) {
		if (!IsValid(S)) continue;
		S->DbgDraw();
	}
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

void AOctTree::Add(const AActor* const Actor) {
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

void AOctTree::DbgDraw() {
	if (!RootNode) return;
	RootNode->DbgDraw();
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
// thas it?