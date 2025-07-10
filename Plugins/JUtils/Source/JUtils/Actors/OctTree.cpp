// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "OctTree.h"

#include "Pool.h"

DEFINE_LOG_CATEGORY_STATIC(LogJOctTree, Log, Log);

// im pulling the algo out of my ... hat. so it's surely not optimized to the max, and also my own copyright.

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
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs: %s a=%s"), __func__, *GetNameSafe(this), *GetNameSafe(Actor));
	if (!IsValid(Actor)) return false;

	const bool Inside = IsInside(Actor);
	UE_CLOG(!Inside, LogJOctTree, Verbose, TEXT("%hs Actor out of my bounds. but i'll take it anyway. lol"), __func__);

	// If it's not inside. we still proceed to insert it. why? because sometimes the box.isnside of a parent passes and the child misses.
	// if the parent decides it's ours. it's ours.

	/// add to self
	// Don't store the actors in this instance if it's split already. wasting a tarray.
	if (UNLIKELY(Nodes.Num()==0)) {
		if (UNLIKELY(Actors.Num()<ActorsMax)) {
			Actors.AddUnique(Actor);
			return true;
		}
		Split();
	}
	
	return AddToNodes(Actor); /// add to sub.
}

int32 AOTNode::Rem(AActor* const Actor) {
	return Actors.RemoveSwap(Actor, EAllowShrinking::No);
}

bool AOTNode::AddToNodes(AActor* const Actor) {
	if (UNLIKELY(!Actor)) return false;
	// using closest because sometimes the parent isInside passes but the sub doesn't.
	// AOTNode* const S = NodeForActor(Actor); // this saves us the trouble of looping and crashing on Add
	// this saves us the trouble of looping and crashing on Add.
	AOTNode* const S = ClosestNode(Actor->GetActorLocation());
	if (UNLIKELY(!S)) return false; // already logged
	return S->Add(Actor); // will trickle down and split. "recursively" (though different objects)
}

void AOTNode::SetBox(const FBox& InBox) {
	UE_CLOG(UNLIKELY(Actors.Num()>0), LogJOctTree, Warning, TEXT("%hs ReBoxing with actors! lol."), __func__);
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
		if (UNLIKELY(!IsValid(S))) continue;
		// maybe there's an optimal way to do this. sorry
		
		NE = E; // extents are already half of the size
		if (i==0) { // "let's start from the top"
			// ignore on i==0. you might think this does nothing. but it skips all the ifs below.
		} else if (i==1) {
			NE.X = -NE.X;
		} else if (i==2) {
			NE.Y = -NE.Y;
		} else if (i==3) {
			NE.Z = -NE.Z;
		} else if (i==4) {
			NE.X = -NE.X;
			NE.Y = -NE.Y;
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
		// this sucks but, it's incredibly important, or the "contains" function will fail.
		// this is how ue does it in its code.
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
		if (UNLIKELY(!N)) continue;
		const float Dist = (To-N->Box.GetCenter()).SizeSquared();
		if (MinDist>Dist) {
			Near = N;
			MinDist = Dist;
		}
	}

	UE_CLOG(UNLIKELY(!Near), LogJOctTree, Warning, TEXT("%hs: %s Could not find it. To=%s"), __func__,
		*GetNameSafe(this), *To.ToString());
	return Near;
}

bool AOTNode::IsInside(AActor* const Actor) const {
	// seems too little for a func, but im sure i'll use it later on.
	if (UNLIKELY(!IsValid(Actor))) return false; // can be called from outside
	const FVector& AT = Actor->GetActorLocation();
	return Box.IsInsideOrOn(AT);
}

AOTNode* AOTNode::Contains(const AActor* const Actor) const {
	// don't care if actor is invalid (but be careful)
	for (const TObjectPtr<AActor>& A: Actors) {
		// i think this is a valid case. the func itself is const.
		if (UNLIKELY(A == Actor)) return const_cast<AOTNode*>(this);
	}

	for (const TObjectPtr<AOTNode>& N: Nodes) {
		if (UNLIKELY(!IsValid(N))) continue; // wtf?

		AOTNode* const R = N->Contains(Actor);
		if (UNLIKELY(R)) return R;
	}

	return nullptr;
}

void AOTNode::PushToNodes() {
	for (const TObjectPtr<AActor>& A: Actors) AddToNodes(A);
	Actors.Empty(); // and these would get disowned.
}

void AOTNode::Split() {
	constexpr uint8 SubsNum = 8;
	UPooler* const Pooler = UPooler::Instance(this);
	// cache because we use it often
	UPool* const Pool = Pooler ? Pooler->GetPool(AOTNode::StaticClass()) : nullptr;
	if (UNLIKELY(!Pooler || !Pool)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs No pooler or no pool. Stop."), __func__);
		return;
	}

	bool Moded = false;
	// 1st create subs if needed
	Nodes.Reserve(SubsNum);
	while (Nodes.Num()<SubsNum) { // should be either 0 or 8. but well.
		AOTNode* const S = Cast<AOTNode>(Pool->Get());
		if (UNLIKELY(!S)) {
			UE_LOG(LogJOctTree, Warning, TEXT("%hs Pool did not return an object. Stop."), __func__);
			return; // no infinite loops plz
		}
		S->SetActorsMax(ActorsMax);
		Nodes.Add(S);
		Moded = true;
	}

	// this is meant to be called with no box, so it's should be true on the good path. but this is a protection.
	if (LIKELY(Moded)) SetNodesBox(); // update the boxes

	PushToNodes(); // then push to them
}

void AOTNode::Reset() {
	Super::Reset();
	// should be empty from the return. most of the things to reset, need to be reset on return
	// Empty(); // already on reset 
}

void AOTNode::SetActorsMax(const int32 InActorsMax) {
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs Max=%i"), __func__, InActorsMax);
	ActorsMax = InActorsMax;
	Actors.Reserve(InActorsMax);
}

void AOTNode::Empty(const bool ReturnSubs) {
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs: %s RetSubs=%i"), __func__, *GetNameSafe(this), ReturnSubs);
	// returning subs first, just in case they do something weird. or i do, in the future.
	if (ReturnSubs) {
		for (const TObjectPtr<AOTNode>& S:Nodes) {
			if (UNLIKELY(!S)) continue;
			S->Return(true);
		}
	}

	Nodes.Empty(8);
	Actors.Empty(ActorsMax); // lol
}

void AOTNode::Return(const bool ReturnSubs) {
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs: %s RetSubs=%i Actors=%i"),
		__func__, *GetNameSafe(this), ReturnSubs, Actors.Num());
	// important to empty on return (instead of reset) to avoid issues with pointers.
	Empty(ReturnSubs);

	UPooler* const Pooler = UPooler::Instance(this);
	if (UNLIKELY(!Pooler)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs can't"), __func__);
		return;
	}
	Pooler->Return(this);

	// doesn't matter.
	// const int32 ANum=Actors.Num();
	// UE_CLOG(ANum>0, LogJOctTree, Warning, TEXT("%hs %s Returning with Actors! n=%i"),
		// __func__, *GetNameSafe(this), ANum);
}

void AOTNode::DbgDraw(const FColor& BoxColor, const FColor& ActorColor, const int32 Size, const float Time) {
	FVector C, E;
	Box.GetCenterAndExtents(C, E);
	DrawDebugBox(GetWorld(), C, E, BoxColor, false, Time, 0, Size);
	for (const TObjectPtr<AActor>& A: Actors) {
		if(UNLIKELY(!IsValid(A))) continue;
		DrawDebugPoint(GetWorld(), A->GetActorLocation(), Size, ActorColor, false, Time, 0);
	}

	for (const TObjectPtr<AOTNode>& S: Nodes) {
		if (UNLIKELY(!IsValid(S))) continue;
		S->DbgDraw(BoxColor, ActorColor, Size, Time);
	}
}

FString AOTNode::ToString() const{
	return FString::Printf(TEXT("%s : Actors=%i Nodes=%i"),
		*GetNameSafe(this), Actors.Num(), Nodes.Num());
}

bool AOTNode::HasLoops() const {
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs: %s Actors=%i"),
		__func__, *GetNameSafe(this), Actors.Num());

	TArray<TObjectPtr<const AOTNode>> Stack;
	Stack.Push(this);
	bool Looping = false;
	for (int32 i =0; i<Stack.Num() && !Looping; ++i) {
		const TObjectPtr<const AOTNode> N = Stack[i];
		if (UNLIKELY(!IsValid(N))) {
			UE_LOG(LogJOctTree, Warning, TEXT("%hs Found invalid node"), __func__);
			continue;
		}
		for (const TObjectPtr<AOTNode>& NN: N->Nodes) {
			if (UNLIKELY(Stack.Contains(NN))) {
				UE_LOG(LogJOctTree, Warning, TEXT("%hs Found loop with node=%s"), __func__, *GetNameSafe(NN));
				Looping = true;
				break;
			}
		}
		Stack.Append(N->Nodes);
	}

	return Looping;
}

bool AOTNode::Iterate(const FJOTIterator& Iterator) {
	if (!Iterator.IsBound()) return true;
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs n=%s"), __func__, *GetNameSafe(this));

	for (const TObjectPtr<AActor>& A: Actors)
		if (UNLIKELY(Iterator.Execute(A, this))) return true;

	for (const TObjectPtr<AOTNode>& S: Nodes)
		if (UNLIKELY(S->Iterate(Iterator))) return true;

	return false;
}

bool AOTNode::IterateIn(const FJOTIterator& Iterator, const FBox& InBox) {
	if (UNLIKELY(!Iterator.IsBound())) return true;
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs n=%s"), __func__, *GetNameSafe(this));
	// const FBox Overlap = Box.Overlap(InBox); // overlap instead of isinside. since we'll check even if close.
	// if (Overlap.GetVolume()<=0) {
	if (LIKELY(!Box.Intersect(InBox))) { // faster than overlap
		UE_LOG(LogJOctTree, Verbose, TEXT("%hs doesn't overlap node=%s"),
			__func__, *GetNameSafe(this));
		return false; // don't break. other nodes might overlap.
	}

	// i could reuse iterate with my own predicate, but it will add overhead and itś not that much code.
	// also the sub calling is different.
	for (const TObjectPtr<AActor>& A: Actors) {
		// if the actor is in it, will execute the iterator. and if the iterator breaks. then break.
		if (UNLIKELY(!IsValid(A))) continue; 
		if (UNLIKELY(InBox.IsInsideOrOn(A->GetActorLocation()) && Iterator.Execute(A, this))) return true;
	}

	for (const TObjectPtr<AOTNode>& S: Nodes) {
		if (UNLIKELY(!IsValid(S))) continue;
		if (UNLIKELY(S->IterateIn(Iterator, InBox))) return true; // bubble break
	}

	return false; // continue the iteration
}

void AOTNode::Pack() {
	if (UNLIKELY(Nodes.Num()==0)) return;

	bool Can = true;
	int32 NumChilds=0;
	for (const TObjectPtr<AOTNode>& N: Nodes) { // this code is similar to tree::add but not quite
		if (UNLIKELY(!N)) continue;
		N->Pack();
		Can = Can & (N->Nodes.Num() == 0); // important to filter the ones with sub nodes
		NumChilds += N->Actors.Num();
	}
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs: %s: pre-pack Can=%i NumChilds=%i"),
		__func__, *GetNameSafe(this), Can, NumChilds);
	
	if (!Can | (NumChilds>=ActorsMax)) return;
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs: %s: packing"), __func__, *GetNameSafe(this));
	for (const TObjectPtr<AOTNode>& N: Nodes) { // this code is similar to tree::add but not quite
		if (UNLIKELY(!N)) continue;
		Actors.Append(N->Actors);
		// shouldn't happen since the above loop filters that.
		UE_CLOG(N->Nodes.Num()>0, LogJOctTree, Verbose, TEXT("%hs: %s: returning with subs!"), __func__, *GetNameSafe(this));
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
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs, a=%s"), __func__, *GetNameSafe(Actor));
	if (UNLIKELY(!RootNode)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return;
	}
	if (UNLIKELY(!IsValid(Actor))) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, invalid actor"), __func__);
		return;
	}
	
	// If it doesn't fit, extend
	if (UNLIKELY(!TryExtend(Actor))) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not extend. disowning."), __func__);
		return; // otherwise rootNode can't handle it.
	}

	RootNode->Add(Actor); // note rootNode and not this->add
}

int32 AOctTree::Rem(AActor* const Actor) const {
	if (UNLIKELY(!RootNode)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return 0;
	}

	// don't care if the actor is valid in this case
	AOTNode* const N = RootNode->Contains(Actor);
	if (UNLIKELY(!N)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs Actor not found in tree. A=%s"),
			__func__, *GetNameSafe(Actor));
		return 0;
	}

	return N->Rem(Actor);
}

bool AOctTree::Update(AActor* const Actor) {
	if (UNLIKELY(!RootNode)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return false;
	}

	AOTNode* const N = RootNode->Contains(Actor);
	if (UNLIKELY(!N)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs Actor not found in tree. A=%s"), __func__, *GetNameSafe(Actor));
		return false;
	}

	if (UNLIKELY(N->IsInside(Actor))) return true; // nothing to do. it's all good still.
	if (UNLIKELY(RootNode->HasLoops())) return false; // TODO test. remove
	if (UNLIKELY(!TryExtend(Actor))) return false;

	N->Rem(Actor);
	const bool Added = RootNode->Add(Actor);
	UE_CLOG(UNLIKELY(!Added), LogJOctTree, Warning, TEXT("%hs couldn't insert the actor %s loc=%s"),
			__func__, *GetNameSafe(Actor), *Actor->GetActorLocation().ToString());
	return Added;
}

void AOctTree::SetBox(const FBox& InBox) {
	bool DoRebuild = true;
	if (UNLIKELY(!RootNode)) {
		RootNode = Cast<AOTNode>(Pool->Get());
		if (!RootNode) {
			UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not create the root. Stop"), __func__);
			return;
		}
		DoRebuild = false; // no need to rebuild
	}

	RootNode->SetActorsMax(ActorsMax);
	RootNode->SetBox(InBox); // tell rebuild which box to use
	if (UNLIKELY(DoRebuild)) Rebuild();
}

void AOctTree::SetActorsMax(const int32 InActorsMax) {
	ActorsMax = InActorsMax;
	if (UNLIKELY(!RootNode)) return;
	
	TArray<TObjectPtr<AOTNode>> Nodes;
	Nodes.Push(RootNode);

	while (Nodes.Num()>0) {
		// if org rootNode is none, it will be skipped here. and above we create one.
		const TObjectPtr<AOTNode>& N = Nodes.Pop(EAllowShrinking::No);
		if (UNLIKELY(!N)) continue;
		N->SetActorsMax(InActorsMax);
		Nodes.Append(N->Nodes);
	}
}

void AOctTree::SetPoolTrimTime(float const InTrimTime) {
	if (UNLIKELY(!Pool)) return;
	Pool->Set(0, AOTNode::StaticClass(), false, true, InTrimTime);
}

void AOctTree::DbgDraw(const FColor& BoxColor, const FColor& ActorColor, int32 const Size, float const Time) {
	if (UNLIKELY(!RootNode)) return;
	RootNode->DbgDraw(BoxColor, ActorColor, Size, Time);
}

void AOctTree::Pack() {
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs"), __func__);
	if (UNLIKELY(!RootNode)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return;
	}

	RootNode->Pack();
}

void AOctTree::Iterate(const FJOTIterator& Iterator) {
	if (UNLIKELY(!RootNode)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return;
	}

	RootNode->Iterate(Iterator);
}

void AOctTree::IterateIn(const FJOTIterator& Iterator, const FBox& Box) {
	if (UNLIKELY(!RootNode)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return;
	}

	RootNode->IterateIn(Iterator, Box);
}

void AOctTree::Print() {
	if (UNLIKELY(!RootNode)) {
		UE_LOG(LogJOctTree, Verbose, TEXT("%hs, could not get the root"), __func__);
		return;
	}

	FJOTIterator I;
	I.BindDynamic(this, &AOctTree::PrintIter);
	Iterate(I);
}

void AOctTree::Rebuild() {
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs"), __func__);
	if (UNLIKELY(!Pool)) {
		UE_LOG(LogJOctTree, Verbose, TEXT("%hs Can't get the Pool. Stop."), __func__);
		return;
	}

	const FBox& Box = RootNode->Box; // cache instead of copy. beware we release the box later.
	TArray<TObjectPtr<AOTNode>> Nodes;
	Nodes.Push(RootNode); // store the old one.
	
	RootNode = Cast<AOTNode>(Pool->Get());
	RootNode->SetActorsMax(ActorsMax); // create a new one. read note below in the loop.
	RootNode->SetBox(Box);

	while (Nodes.Num()>0) {
		// if org rootNode is none, it will be skipped here. and above we create one.
		AOTNode* const N = Nodes.Pop(EAllowShrinking::No);
		UE_LOG(LogJOctTree, Verbose, TEXT("%hs N=%s"), __func__, *GetNameSafe(N));
		if (UNLIKELY(!N)) continue;

		UE_LOG(LogJOctTree, Verbose, TEXT("%hs N=%s An=%i"), __func__, *GetNameSafe(N), N->Actors.Num());
		
		// steal nodes (before 'add' case it ends up using one of those nodes)
		// (though it should not have actors if it has nodes)
		Nodes.Append(N->Nodes); // note this is the stack not N

		// steal actors
		// notice we use this->Add. this will trigger TryExtend. which can and will change the RootActor
		// at first glance you should be concerned that this will mess with the nodes in the list Nodes.
		// but we create a NEW tree while keeping the old one in memory, and we navigate the old one, while the new one is in place.
		for (const TObjectPtr<AActor>& A: N->Actors) Add(A);
		N->Return(false); // we stole them. return will return them too, otherwise
	}
}

AOTNode* AOctTree::Contains(const AActor* const A) const {
	if (UNLIKELY(!RootNode)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, No root node. Stop"), __func__);
		return nullptr;
	}

	return RootNode->Contains(A);
}

bool AOctTree::HasLoops() const {
	return !TestForLoops || (RootNode && UNLIKELY(RootNode->HasLoops()));
}

void AOctTree::BeginPlay() {
	Super::BeginPlay();

	UPooler* const Pooler = UPooler::Instance(this);
	if (UNLIKELY(!Pooler)){
		UE_LOG(LogJOctTree, Warning, TEXT("Could not obtain the Pooler. this would crash later."));
		return;
	}

	Pool = Pooler->SetPool(0, AOTNode::StaticClass(), false, true, 1);
	UE_CLOG(UNLIKELY(!Pool), LogJOctTree, Warning, TEXT("Could not obtain the Pool. this would crash later."));
}

void AOctTree::DestroyPool() {
	UPooler* const Pooler = UPooler::Instance(this);
	// destroy the pool before returning the nodes. that way they'll get destroyed upon return. avoiding extra overhead.
	if (LIKELY(Pooler)) Pooler->RemPool(AOTNode::StaticClass()); // will empty the pool and destroy it.
}

void AOctTree::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	// DestroyPool(); // actually no. because there might be other octtrees
	if (LIKELY(RootNode)) RootNode->Return(true); // will return all of them

	// empty after returning to also delete the ones returned.
	if (LIKELY(Pool)) Pool->Empty(); // might affect performance, but...
	Pool = nullptr;

	RootNode = nullptr;

	// Return all nodes
	Super::EndPlay(EndPlayReason);
}

bool AOctTree::PrintIter(AActor* const A, AOTNode* const Node) {
	UE_LOG(LogJOctTree, Log, TEXT("%hs A=%s N=%s"), __func__, *GetNameSafe(A), *GetNameSafe(Node));
	return false;
}

bool AOctTree::TryExtend(AActor* const Actor) {
	UE_LOG(LogJOctTree, Verbose, TEXT("%hs A=%s"), __func__, *GetNameSafe(Actor));
	if (UNLIKELY(!RootNode || !Actor)) return false;

	int32 Loop = ExtendMax;
	while (Loop>0) {
		UE_LOG(LogJOctTree, Verbose, TEXT("%hs loop=%i"), __func__, Loop);
		--Loop;
		
		if (UNLIKELY(RootNode->IsInside(Actor))) {
			UE_LOG(LogJOctTree, Verbose, TEXT("%hs loop=%i it's inside"), __func__, Loop);
			return true;
		}

		UE_LOG(LogJOctTree, Verbose, TEXT("%hs loop=%i Check A"), __func__, Loop);
		AOTNode* const NewRoot = Cast<AOTNode>(Pool->Get());
		if (UNLIKELY(!NewRoot)) return false;

		UE_LOG(LogJOctTree, Verbose, TEXT("%hs loop=%i Check B"), __func__, Loop);
		NewRoot->SetActorsMax(ActorsMax);

		const FBox& RBox = RootNode->Box;
		// find out which way we need to go
		const FVector RCenter = RBox.GetCenter();
		const FVector Ext = RBox.GetExtent();
		const FVector APos = Actor->GetActorLocation();
		const FVector Diff = APos - RCenter; // end-start
		const FVector Sign = Diff.GetSignVector();
		// const bool BDir[] = {Dir.X>=0, Dir.Y>=0, Dir.Z>=0}; // i could optimize with bit manip

		// calculate parents center and extent and others
		const FVector ExtS = Ext*Sign;
		const FVector PCent = RCenter+ExtS;
		const FVector PExt = Ext*2;
		const FVector PMax = PCent+PExt;

		// set the new box
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
		UE_LOG(LogJOctTree, Verbose, TEXT("%hs loop=%i PBox=%s"), __func__, Loop, *PBox.ToString());

		// avoid having to calculate the extent for the children based on the above node.
		// and just re-do them all. suboptimal for the cpu. optimal for me.
		NewRoot->Split();
		
		// we'll just find whichever node is the corresponding to the current one and clone it.
		// this is the best way, and will 100% return the one corresponding.
		// using overlap or isinside is more complex and not more accurate
		AOTNode* const NCloser = NewRoot->ClosestNode(RCenter);
		if (UNLIKELY(!NCloser)) return false;
		
		// clone it // TODO move to node, maybe?
		NCloser->Actors = RootNode->Actors;
		NCloser->Nodes = RootNode->Nodes;
		RootNode->Return(false); // false because we stole them
		RootNode = NewRoot;
	}

	return false;
}

// that's it?
