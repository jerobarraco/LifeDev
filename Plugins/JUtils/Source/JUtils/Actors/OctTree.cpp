// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "OctTree.h"

#include "Pool.h"

DEFINE_LOG_CATEGORY_STATIC(LogJOctTree, Log, Log);

// im pulling the algo out of my ... hat.

// TODO remove actor
// // TODO collapse "unsplit" nodes
// TODO query nodes based on position maybe distance?
// // TODO Get all nodes in this box (use Box.Overlaps)
// // TODO use a delegate like the iterator
// TODO rebuild tree
// TODO resize
// TODO pack
// TODO update tree based on actors changing.
// // TODO start with a naive approach and update all of them
// TODO when adding an actor, to the tree. if it doesn't overlap the root, create a new root with subs
// TODO pass parent to children
// TODO can be optimized with a TRingBuffer does not count ref for uobject pointer, they could become stale :(

AOTNode::AOTNode() {
	Super::SetActorTickEnabled(false);
	PrimaryActorTick.SetTickFunctionEnable(false);
	PrimaryActorTick.bStartWithTickEnabled = false;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void AOTNode::Add(AActor* const Actor, AOTNode* NotTo) {
	// TODO test bounds and reject the rejected
	// clog rulz, ok.
	UE_CLOG(!IsInside(Actor), LogJOctTree, Warning, TEXT("%hs Actor out of my bounds. but i'll take it anyway. lol"), __func__);
	// Dont store the actors in this instance if it's split already. wasting a tarray.
	if (Nodes.Num()==0) {
		if (Actors.Num()<ActorsMax) {
			Actors.Add(Actor);
			return;
		}
		Split();
	}

	AddToSub(Actor); //pass notto
}

int32 AOTNode::Rem(AActor* const Actor) {
	return Actors.RemoveSwap(Actor, EAllowShrinking::No);
}

void AOTNode::AddToSub(AActor* const Actor) {
	AOTNode* const S = NodeForActor(Actor);
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
	const int32 Num = Nodes.Num();
	FVector C, E, Max, NE, Min;
	Box.GetCenterAndExtents(C, E);
	// surely ill need it to update the bounds if i ever do implement that
	for (uint8 i=0; i<Num; ++i) {
		AOTNode* const S = Nodes[i];
		if (!IsValid(S)) continue;
		// maybe there's an optimal way to do this. sorry
		
		// im gonna use min as C for all of them so what? i hope ue will normalize my lame-ness. whats min and max in 3d anyway?
		// im sure itll bite me
		NE = E; // extents are already half of the size
		if (i==0) { // "let's start from the top"
		} else if (i==1) {
			NE.X = -NE.X;
		} else if (i==2) {
			NE.Y = -NE.Y;
		} else if (i==3) {
			NE.Z = -NE.Z;
		} else if (i==4) {
			NE.X = -NE.X; // this can be optimized but i dont feel like now. apollo-gies
			NE.Y = -NE.Y; // this can be optimized but i dont feel like now. apollo-gies
		} else if (i==5) {
			NE.X = -NE.X;
			NE.Z = -NE.Z;
		} else if (i==6) {
			NE.Y = -NE.Y;
			NE.Z = -NE.Z;
		} else if (i==7) {
			NE.X = -NE.X;
			NE.Y = -NE.Y;
			NE.Z = -NE.Z;
		}// without thinking it too much, it fits....
		
		const FVector B = C+NE;
		// this sucks but it's incredibly important, or the "contains" function will fail.
		// TODO En-better this.
		Min.X = FMath::Min(C.X, B.X);
		Min.Y = FMath::Min(C.Y, B.Y);
		Min.Z = FMath::Min(C.Z, B.Z);
		Max.X = FMath::Max(C.X, B.X);
		Max.Y = FMath::Max(C.Y, B.Y);
		Max.Z = FMath::Max(C.Z, B.Z);

		S->SetBox(FBox(Min, Max));
	}
}

AOTNode* AOTNode::NodeForActor(AActor* const Actor) {
	for (AOTNode* const S: Nodes) {
		if (IsValid(S) && S->IsInside(Actor)) return S;
	}

	UE_LOG(LogJOctTree, Warning, TEXT("%hs Could not find it"), __func__);
	return nullptr;
}

bool AOTNode::IsInside(AActor* const Actor) const {
	if (!IsValid(Actor)) return false; // seems too little for a func, but im sure ill use it later on.
	const FVector& AT = Actor->GetActorLocation();
	return Box.IsInsideOrOn(AT);
}

AOTNode* AOTNode::Contains(AActor* const Actor) const {
	for (AActor* const A: Actors) {
		// i think this is a valid case. the func itself is const.
		if (A == Actor) return const_cast<AOTNode*>(this);
	}

	for (AOTNode* const N: Nodes) {
		if (!IsValid(N)) continue; // wtf?

		AOTNode* const R = N->Contains(Actor);
		if (R) return R;
	}

	return nullptr;
}

void AOTNode::PushToSubs() {
	// 2nd move the actors to subs
	for (AActor* const A: Actors) {
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

	bool Moded = false;
	// WTF DEGENERATE CASE! but meh
	// 1st create subs
	while (Nodes.Num()<SubsNum) {
		AOTNode* const S = Cast<AOTNode>(Pool->Get());
		if (!S) {
			UE_LOG(LogJOctTree, Warning, TEXT("%hs can't 2 "), __func__);
			return;
		}
		S->ActorsMax = ActorsMax; // TODO create S->SetUp func,pass parent
		Nodes.Add(S);
		Moded = true;
	}
	if (Moded)
		SetSubsBox();

	PushToSubs();
}

void AOTNode::Reset() {
	Super::Reset();
	// Empty(); // should be empty from the return. 
}

void AOTNode::Empty() {
	for (AOTNode* const S:Nodes) {
		if (!S) continue;
		S->Return();
	}
	Nodes.Empty();
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
		DrawDebugPoint(GetWorld(), A->GetActorLocation(), 5, FColor::Yellow, false, 1, 0);
	}

	for (AOTNode* const S: Nodes) {
		if (!IsValid(S)) continue;
		S->DbgDraw();
	}
}

bool AOTNode::Iterate(const FJOTIterator& Iterator) {
	if (!Iterator.IsBound()) return true;
	UE_LOG(LogJOctTree, Log, TEXT("%hs"), __func__);

	for (AActor* const A: Actors)
		if (Iterator.Execute(A, this)) return true;

	for (AOTNode* const S: Nodes)
		if (S->Iterate(Iterator)) return true;

	return false;
}

bool AOTNode::IterateInside(const FJOTIterator& Iterator, const FBox& InBox) {
	if (!Iterator.IsBound()) return true;
	UE_LOG(LogJOctTree, Log, TEXT("%hs"), __func__);
	const FBox Overlap = Box.Overlap(InBox);
	if (Overlap.GetVolume()<=0) {
		UE_LOG(LogJOctTree, Log, TEXT("%hs doesn't overlap Over=%s node=%s"),
			__func__, *Overlap.ToString(), *GetNameSafe(this));
		return false; // don´t break.
	}

	// i could reuse iterate with my own predicate but it will add overhead and itś not that much code.
	// also the sub calling is different.
	// TODO getactorlocation could be memoized
	// // TODO make a memoizer??
	for (AActor* const A: Actors) {
		// if the actor is in it, will execute the iterator. and if the iterator breaks. then break.
		if (IsValid(A) && InBox.IsInsideOrOn(A->GetActorLocation()) && Iterator.Execute(A, this)) return true;
	}

	for (AOTNode* const S: Nodes)
		if (IsValid(S) && S->IterateInside(Iterator, InBox)) return true; // bubble break

	return false; // continue the iteration
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

void AOctTree::Add(AActor* const Actor) {
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("could not get the root"));
		return;
	}

	RootNode->Add(Actor);
}

int32 AOctTree::Rem(AActor* const Actor) {
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return 0;
	}

	AOTNode* const N = RootNode->Contains(Actor);
	if (!N) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs Actor not found in tree. O=%s"), __func__, *GetNameSafe(Actor));
		return 0;
	}

	return N->Rem(Actor);
}

void AOctTree::SetBox(const FBox& InBox) {
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return;
	}

	RootNode->SetBox(InBox);
}

void AOctTree::DbgDraw() {
	if (!RootNode) return;
	RootNode->DbgDraw();
}

void AOctTree::Iterate(const FJOTIterator& Iterator) const {
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return;
	}

	RootNode->Iterate(Iterator);
}

void AOctTree::Print() {
	if (!RootNode) return;
	FJOTIterator I;
	I.BindDynamic(this, &AOctTree::PrintIter);
	Iterate(I);
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

bool AOctTree::PrintIter(AActor* const A, AOTNode* const Node) {
	UE_LOG(LogJOctTree, Log, TEXT("%hs A=%s N=%s"), __func__, *GetNameSafe(A), *GetNameSafe(Node));
	return false;
}
// thas it?
