// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "OctTree.h"

#include "Pool.h"

DEFINE_LOG_CATEGORY_STATIC(LogJOctTree, Log, Log);

// im pulling the algo out of my ... hat.

// TODO when adding an actor, to the tree. if it doesn't overlap the root, create a new root with subs

AOTNode::AOTNode() {
	Super::SetActorTickEnabled(false);
	PrimaryActorTick.SetTickFunctionEnable(false);
	PrimaryActorTick.bStartWithTickEnabled = false;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

bool AOTNode::Add(AActor* const Actor) {
	// does not check for isvalid. that is checked by the tree. little, bit of ... optimization.
	// the tree is justifying its existence...
	// clog rulz, ok.
	const bool Inside = IsInside(Actor);
	
	UE_LOG(LogJOctTree, Log, TEXT("%hs: %s a=%s"), __func__, *GetNameSafe(this), *GetNameSafe(Actor));
	UE_CLOG(!Inside, LogJOctTree, Warning, TEXT("%hs Actor out of my bounds. but i'll take it anyway. lol"), __func__);

	if (!IsValid(Actor)) return false;

	// If it's not inside. we still proceed to insert it. why? because sometimes the box.isnside of a parent passes and the child misses.
	/// add to self
	// Don't store the actors in this instance if it's split already. wasting a tarray.
	if (Nodes.Num()==0) {
		if (Actors.Num()<ActorsMax) {
			Actors.AddUnique(Actor);
			return true;
		}
		Split();
	}

	/// add to sub. This might loop. but add to sub checks for inside before calling add
	return AddToNodes(Actor);
}

int32 AOTNode::Rem(AActor* const Actor) {
	return Actors.RemoveSwap(Actor, EAllowShrinking::No);
}

bool AOTNode::AddToNodes(AActor* const Actor) {
	// using closest because sometimes the parent isinside passes but the sub doesnt.
	// AOTNode* const S = NodeForActor(Actor); // this saves us the trouble of looping and crashing on Add
	AOTNode* const S = ClosestNode(Actor->GetActorLocation()); // this saves us the trouble of looping and crashing on Add
	if (!S) return false; // already logged
	return S->Add(Actor); // will trickle down and split. "recursively" (though different objects)
}

void AOTNode::SetBox(const FBox& InBox) {
	if (Actors.Num()>0)
		UE_LOG(LogJOctTree, Warning, TEXT("%hs Rebounding with actors. lol."), __func__);
	// why bother. this is not meant to be optimal yet
	Box = InBox;
	Box.IsValid = true; // because unreal
}

void AOTNode::SetNodesBox() {
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

AOTNode* AOTNode::ClosestNode(const FVector& To) {
	AOTNode* Near = nullptr;
	float MinDist = INFINITY;
	for (AOTNode* const N: Nodes) {
		if (!N) continue;
		const float Dist = (To-N->Box.GetCenter()).SizeSquared();
		if (MinDist>Dist) {
			Near = N;
			MinDist = Dist;
		}
	}
	UE_CLOG(!Near, LogJOctTree, Warning, TEXT("%hs: %s Could not find it. To=%s "), __func__,
		*GetNameSafe(this), *To.ToString());
	return Near;
}

bool AOTNode::IsInside(AActor* const Actor) const {
	// seems too little for a func, but im sure ill use it later on.
	if (!IsValid(Actor)) return false; // can be called from outside
	const FVector& AT = Actor->GetActorLocation();
	return Box.IsInsideOrOn(AT);
}

AOTNode* AOTNode::Contains(AActor* const Actor) const {
	// don't care if actor is invalid (but be careful)
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

void AOTNode::PushToNodes() {
	// 2nd move the actors to subs
	for (AActor* const A: Actors) AddToNodes(A); // this could trigger addtoParent though.
	Actors.Empty(); // and these would get disowned.
}

void AOTNode::Split() {
	constexpr uint8 SubsNum = 8;
	UPooler* const Pooler = UPooler::Instance(this);
	UPool* const Pool = Pooler ? Pooler->GetPool(AOTNode::StaticClass()) : nullptr;
	if (!Pooler || !Pool) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs can't"), __func__);
		return;
	}

	bool Moded = false;
	// WTF DEGENERATE CASE! but meh
	// 1st create subs
	Nodes.Reserve(SubsNum);
	while (Nodes.Num()<SubsNum) {
		AOTNode* const S = Cast<AOTNode>(Pool->Get());
		if (!S) {
			UE_LOG(LogJOctTree, Warning, TEXT("%hs can't 2 "), __func__);
			return; // no infinite loops plz
		}
		S->SetUp(ActorsMax);
		Nodes.Add(S);
		Moded = true;
	}
	if (Moded)
		SetNodesBox();

	PushToNodes();
}

void AOTNode::Reset() {
	Super::Reset();
	// Empty(); // should be empty from the return. 
}

void AOTNode::SetUp(const int32 Max) {
	ActorsMax = Max;
	Actors.Reserve(Max);
}

void AOTNode::Empty(const bool ReturnSubs) {
	UE_LOG(LogJOctTree, Log, TEXT("%hs: %s RSubs=%i"), __func__, *GetNameSafe(this), ReturnSubs);
	if (ReturnSubs) { // returning before, just in case the children do something weird. or i do in the future.
		for (AOTNode* const S:Nodes) {
			if (!S) continue;
			S->Return(true);
		}
	}

	Nodes.Empty(8);
	Actors.Empty(ActorsMax); // lol
}

void AOTNode::Return(const bool ReturnSubs) {
	UE_LOG(LogJOctTree, Log, TEXT("%hs: %s RSubs=%i Actors=%i"),
		__func__, *GetNameSafe(this), ReturnSubs, Actors.Num());
	Empty(ReturnSubs);

	UPooler* const Pooler = UPooler::Instance(this);
	if (!Pooler) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs can't"), __func__);
		return;
	}
	Pooler->Return(this);

	// doesn't matter.
	// const int32 ANum=Actors.Num();
	// UE_CLOG(ANum>0, LogJOctTree, Warning, TEXT("%hs %s Returning with Actors! n=%i"),
		// __func__, *GetNameSafe(this), ANum);
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
	UE_LOG(LogJOctTree, Log, TEXT("%hs n=%s"), __func__, *GetNameSafe(this));

	for (AActor* const A: Actors)
		if (Iterator.Execute(A, this)) return true;

	for (AOTNode* const S: Nodes)
		if (S->Iterate(Iterator)) return true;

	return false;
}

bool AOTNode::IterateIn(const FJOTIterator& Iterator, const FBox& InBox) {
	if (!Iterator.IsBound()) return true;
	UE_LOG(LogJOctTree, Log, TEXT("%hs n=%s"), __func__, *GetNameSafe(this));
	const FBox Overlap = Box.Overlap(InBox); // overlap instead of isinside. since we'll check even if close.
	if (Overlap.GetVolume()<=0) {
		UE_LOG(LogJOctTree, Log, TEXT("%hs doesn't overlap Over=%s node=%s"),
			__func__, *Overlap.ToString(), *GetNameSafe(this));
		return false; // don't break. other nodes might overlap.
	}

	// i could reuse iterate with my own predicate, but it will add overhead and itś not that much code.
	// also the sub calling is different.
	for (AActor* const A: Actors) {
		// if the actor is in it, will execute the iterator. and if the iterator breaks. then break.
		if (IsValid(A) && InBox.IsInsideOrOn(A->GetActorLocation()) && Iterator.Execute(A, this)) return true;
	}

	for (AOTNode* const S: Nodes)
		if (IsValid(S) && S->IterateIn(Iterator, InBox)) return true; // bubble break

	return false; // continue the iteration
}

void AOTNode::Pack() {
	if (Nodes.Num()==0) return;

	bool Can = true;
	int32 NumChilds=0;
	for (AOTNode* const N: Nodes) { // this code is similar to tree::add but not quite
		if (!N) continue;;
		if (!N) continue;
		N->Pack();
		Can = Can && N->Nodes.Num() == 0;
		NumChilds += N->Actors.Num();
	}
	UE_LOG(LogJOctTree, Log, TEXT("%hs: %s: pre-pack Can=%i NumChilds=%i"),
		__func__, *GetNameSafe(this), Can, NumChilds);
	
	if (!Can || NumChilds>=ActorsMax) return;
	UE_LOG(LogJOctTree, Log, TEXT("%hs: %s: packing"), __func__, *GetNameSafe(this));
	for (AOTNode* const N: Nodes) { // this code is similar to tree::add but not quite
		if (!N) continue;
		Actors.Append(N->Actors);
		
		UE_CLOG(N->Nodes.Num()>0, LogJOctTree, Log, TEXT("%hs: %s: returning with subs!"), __func__, *GetNameSafe(this));
		N->Return(true);
	}
	Nodes.Empty(8);
}

void AOTNode::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Empty(true);
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
	UE_LOG(LogJOctTree, Log, TEXT("%hs, a=%s"), __func__, *GetNameSafe(Actor));
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return;
	}
	if (!IsValid(Actor)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, invalid actor"), __func__);
		return;
	}
	
	// If it doesn't fit, extend
	if (!TryExtend(Actor)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not extend. disowning."), __func__);
		return; // otherwise rootnode will loop right?
	}
	RootNode->Add(Actor);
}

int32 AOctTree::Rem(AActor* const Actor) {
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return 0;
	}

	// don't care if the actor is valid in this case
	AOTNode* const N = RootNode->Contains(Actor);
	if (!N) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs Actor not found in tree. A=%s"), __func__, *GetNameSafe(Actor));
		return 0;
	}

	return N->Rem(Actor);
}

bool AOctTree::Update(AActor* const Actor) {
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return false;
	}

	AOTNode* const N = RootNode->Contains(Actor);
	if (!N) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs Actor not found in tree. A=%s"), __func__, *GetNameSafe(Actor));
		return false;
	}

	if (N->IsInside(Actor)) return true;
	if (!TryExtend(Actor)) return false;

	N->Rem(Actor);
	const bool Added = RootNode->Add(Actor);
	if (!Added)
		UE_LOG(LogJOctTree, Error, TEXT("%hs couldnt insert the actor %s loc=%s"),
			__func__, *GetNameSafe(Actor), *Actor->GetActorLocation().ToString());
	return Added;
}

void AOctTree::SetBox(const FBox& InBox) {
	if (!RootNode) {
		RootNode = Cast<AOTNode>(Pool->Get());
		if (!RootNode) {
			UE_LOG(LogJOctTree, Error, TEXT("%hs, could not create the root. Stop"), __func__);
			return;
		}

		RootNode->SetUp(ActorsMax);
		RootNode->SetBox(InBox);
		return; // no need to rebuild
	}
	RootNode->SetBox(InBox); // tell rebuild which box to use
	Rebuild();
}

void AOctTree::SetActorsMax(const int32 InActorsMax) {
	ActorsMax = InActorsMax;
	if (!RootNode) return;
	
	TArray<AOTNode*> Nodes;
	Nodes.Push(RootNode);

	while (Nodes.Num()>0) {
		// if org rootNode is none, it will be skipped here. and above we create one.
		AOTNode* const N = Nodes.Pop(EAllowShrinking::No);
		if (!N) continue;
		N->ActorsMax = InActorsMax;
		Nodes.Append(N->Nodes);
	}
}

void AOctTree::SetPoolTrimTime(float const InTrimTime) {
	if (!Pool) return;
	Pool->Set(0, AOTNode::StaticClass(), false, true, InTrimTime);
}

void AOctTree::DbgDraw() {
	if (!RootNode) return;
	RootNode->DbgDraw();
}

void AOctTree::Pack() {
	UE_LOG(LogJOctTree, Log, TEXT("%hs"), __func__);
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return;
	}

	RootNode->Pack();
}

void AOctTree::Iterate(const FJOTIterator& Iterator) {
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return;
	}

	RootNode->Iterate(Iterator);
}

void AOctTree::IterateIn(const FJOTIterator& Iterator, const FBox& Box) {
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return;
	}
	RootNode->IterateIn(Iterator, Box);
}

void AOctTree::Print() {
	if (!RootNode) return;
	FJOTIterator I;
	I.BindDynamic(this, &AOctTree::PrintIter);
	Iterate(I);
}

void AOctTree::Rebuild() {
	UE_LOG(LogJOctTree, Log, TEXT("%hs"), __func__);
	if (!Pool) return;

	const FBox& Box = RootNode->Box; // cache instead of copy. beware we release the box later.
	TArray<AOTNode*> Nodes;
	Nodes.Push(RootNode);
	
	RootNode = Cast<AOTNode>(Pool->Get());
	RootNode->SetUp(ActorsMax);
	RootNode->SetBox(Box);

	while (Nodes.Num()>0) {
		// if org rootNode is none, it will be skipped here. and above we create one.
		AOTNode* const N = Nodes.Pop(EAllowShrinking::No);
		UE_LOG(LogJOctTree, Log, TEXT("%hs N=%s"), __func__, *GetNameSafe(N));
		if (!N) continue;
		UE_LOG(LogJOctTree, Log, TEXT("%hs N=%s An=%i"), __func__, *GetNameSafe(N), N->Actors.Num());
		
		// steal nodes (before 'add' case it ends up using one of those nodes)
		// (though it should not have actors if it has nodes)
		Nodes.Append(N->Nodes); // note this is the stack not N

		for (AActor* const A: N->Actors) Add(A); // steal actors
		N->Return(false); // we stole them. return will return them too, otherwise
	}
}

void AOctTree::BeginPlay() {
	Super::BeginPlay();

	UPooler* const Pooler = UPooler::Instance(this);
	if (!Pooler) {
		UE_LOG(LogJOctTree, Warning, TEXT("Could not obtain the Pooler. this would crash later."));
		return;
	}

	Pool = Pooler->SetPool(1, AOTNode::StaticClass(), false, true, 1);
	UE_CLOG(!Pool, LogJOctTree, Warning, TEXT("Could not obtain the Pool. this would crash later."));
}

void AOctTree::DestroyPool() {
	UPooler* const Pooler = UPooler::Instance(this);
	// destroy the pool before returning the nodes. that way they'll get destroyed upon return. avoiding extra overhead.
	if (Pooler) Pooler->RemPool(AOTNode::StaticClass()); // will empty the pool and destroy it.
}

void AOctTree::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Pool = nullptr;
	// DestroyPool(); // actually no. because there might be other octtrees
	if (RootNode) RootNode->Return(true); // will return all of them
	
	RootNode = nullptr;

	// Return all nodes
	Super::EndPlay(EndPlayReason);
}

bool AOctTree::PrintIter(AActor* const A, AOTNode* const Node) {
	UE_LOG(LogJOctTree, Log, TEXT("%hs A=%s N=%s"), __func__, *GetNameSafe(A), *GetNameSafe(Node));
	return false;
}

bool AOctTree::TryExtend(AActor* Actor) {
	UE_LOG(LogJOctTree, Log, TEXT("%hs A=%s"), __func__, *GetNameSafe(Actor));
	if (!RootNode || !Actor) return false;

	int32 Loop = ExtendMax;
	while (Loop>0) {
		UE_LOG(LogJOctTree, Log, TEXT("%hs loop=%i"), __func__, Loop);
		--Loop;
		
		if (RootNode->IsInside(Actor)) {
			UE_LOG(LogJOctTree, Log, TEXT("%hs loop=%i it's inside"), __func__, Loop);
			return true;
		}

		UE_LOG(LogJOctTree, Log, TEXT("%hs loop=%i Check A"), __func__, Loop);
		AOTNode* const NewRoot = Cast<AOTNode>(Pool->Get());
		if (!NewRoot) return false;
		UE_LOG(LogJOctTree, Log, TEXT("%hs loop=%i Check B"), __func__, Loop);

		const FBox& RBox = RootNode->Box;
		// find out which way we need to go
		const FVector RCenter = RBox.GetCenter();
		const FVector Ext = RBox.GetExtent();
		const FVector APos = Actor->GetActorLocation();
		const FVector Diff = APos - RCenter; // end-start
		const FVector Sign = Diff.GetSignVector();
		// const bool BDir[] = {Dir.X>=0, Dir.Y>=0, Dir.Z>=0}; // i could optimize with bit manip
		// this might work. or maybe is just nonsense
		const FVector ExtS = Ext*Sign;
		const FVector PCent = RCenter+ExtS;
		const FVector PExt = Ext*2;
		const FVector PMax = PCent+PExt;
		FBox& PBox = NewRoot->Box; // alias
		PBox.Min = PCent-PExt;
		// reusing parboxmin.
		PBox.Max.X = FMath::Max(PBox.Min.X, PMax.X);
		PBox.Max.Y = FMath::Max(PBox.Min.Y, PMax.Y);
		PBox.Max.Z = FMath::Max(PBox.Min.Z, PMax.Z);
		// reusing parboxmax
		PBox.Min.X = FMath::Min(PBox.Min.X, PBox.Max.X);
		PBox.Min.Y = FMath::Min(PBox.Min.Y, PBox.Max.Y);
		PBox.Min.Z = FMath::Min(PBox.Min.Z, PBox.Max.Z);
		PBox.IsValid = 1; // Because unreal, that's why.
		UE_LOG(LogJOctTree, Log, TEXT("%hs loop=%i PBox=%s"), __func__, Loop, *PBox.ToString());

		NewRoot->Split(); // avoid having to calculate the extent for the children based on the above node.
		
		// this is a hack might not work well
		AOTNode* const NCloser = NewRoot->ClosestNode(RCenter); 
		if (!NCloser) return false;
		
		// clone it // TODO move to node
		NCloser->Actors = RootNode->Actors;
		NCloser->Nodes = RootNode->Nodes;
		RootNode->Return(false); // we stole them
		RootNode = NewRoot;
	}

	return false;
}
// thas it?
