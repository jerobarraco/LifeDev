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

bool AOTNode::Add(AActor* const Actor, AOTNode* NotTo) {
	// does not check for isvalid. that is checked by the tree. little, bit of ... optimization.
	// the tree is justifying its existence...
	// clog rulz, ok.
	const bool Inside = IsInside(Actor);
	UE_CLOG(!Inside, LogJOctTree, Warning, TEXT("%hs Actor out of my bounds. but i'll take it anyway. lol"), __func__);
	/// ask dad for halp
	if (!Inside) {
		AddToParent(Actor);
		return true; // assume handled. avoid loops.
	}

	/// add to self
	// Don't store the actors in this instance if it's split already. wasting a tarray.
	if (Nodes.Num()==0) {
		if (Actors.Num()<ActorsMax) {
			Actors.AddUnique(Actor);
			return false;
		}
		Split();
	}

	/// add to sub. This might loop. but add to sub checks for inside before calling add
	return AddToSub(Actor, NotTo);
}

int32 AOTNode::Rem(AActor* const Actor) {
	return Actors.RemoveSwap(Actor, EAllowShrinking::No);
}

bool AOTNode::AddToSub(AActor* const Actor, AOTNode* const NotTo) {
	AOTNode* const S = NodeForActor(Actor, NotTo); // this saves us the trouble of looping and crashing on Add
	if (!S) return false; // already logged
	return S->Add(Actor); // will trickle down and split. "recursively" (though different objects)
}

void AOTNode::SetBox(const FBox& InBox) {
	if (Actors.Num()>0)
		UE_LOG(LogJOctTree, Warning, TEXT("%hs Rebounding with actors. lol."), __func__);
	// why bother. this is not meant to be optimal yet
	Box = InBox;
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

AOTNode* AOTNode::NodeForActor(AActor* const Actor, AOTNode* const NotOn) {
	for (AOTNode* const S: Nodes) {
		if (S != NotOn && IsValid(S) && S->IsInside(Actor)) return S;
	}

	UE_LOG(LogJOctTree, Warning, TEXT("%hs Could not find it"), __func__);
	return nullptr;
}

void AOTNode::AddToParent(AActor* const Actor) {
	// ret void to avoid loops
	if (!Parent) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs: %s: need a parent, but has none."),
				__func__, *GetNameSafe(this));
		return;
	}

	Parent->Add(Actor, this);
}

bool AOTNode::Update(AActor* Actor) {
	// can be called directly from bp, through contains
	if (!IsValid(Actor)) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, invalid actor"), __func__);
		return false;
	}

	// opt: not calling Add(Actor) to avoid removing and re-adding in vain 
	if (IsInside(Actor)) return true; // nothing to do.
	Actors.Remove(Actor); // disown
	AddToParent(Actor);
	return true; // assume handled. avoid loops.
}

bool AOTNode::IsInside(AActor* const Actor) const {
	if (!IsValid(Actor)) return false; // seems too little for a func, but im sure ill use it later on.
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

void AOTNode::PushToSubs() {
	// 2nd move the actors to subs
	for (AActor* const A: Actors) AddToSub(A);
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
	Nodes.Reserve(SubsNum);
	while (Nodes.Num()<SubsNum) {
		AOTNode* const S = Cast<AOTNode>(Pool->Get());
		if (!S) {
			UE_LOG(LogJOctTree, Warning, TEXT("%hs can't 2 "), __func__);
			return;
		}
		S->SetUp(this, ActorsMax);
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

void AOTNode::SetUp(AOTNode* const InParent, const int32 Max) {
	Parent = InParent;
	ActorsMax = Max;
	Actors.Reserve(Max);
}

void AOTNode::Empty(const bool ReturnSubs) {
	if (!ReturnSubs) { // returning before, just in case the children do something weird. or i do in the future.
		for (AOTNode* const S:Nodes) {
			if (!S) continue;
			S->Return(true);
		}
	}
	Nodes.Empty(8);
	Actors.Empty(ActorsMax); // lol
}

void AOTNode::Return(bool ReturnSubs) {
	UPooler* const Pooler = UPooler::Instance(this);
	if (!Pooler) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs can't"), __func__);
		return;
	}

	Empty(ReturnSubs);
	Parent = nullptr;
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
	UE_LOG(LogJOctTree, Log, TEXT("%hs n=%s"), __func__, *GetNameSafe(this));

	for (AActor* const A: Actors)
		if (Iterator.Execute(A, this)) return true;

	for (AOTNode* const S: Nodes)
		if (S->Iterate(Iterator)) return true;

	return false;
}

bool AOTNode::IterateInside(const FJOTIterator& Iterator, const FBox& InBox) {
	if (!Iterator.IsBound()) return true;
	UE_LOG(LogJOctTree, Log, TEXT("%hs n=%s"), __func__, *GetNameSafe(this));
	const FBox Overlap = Box.Overlap(InBox);
	if (Overlap.GetVolume()<=0) {
		UE_LOG(LogJOctTree, Log, TEXT("%hs doesn't overlap Over=%s node=%s"),
			__func__, *Overlap.ToString(), *GetNameSafe(this));
		return false; // don´t break.
	}

	// i could reuse iterate with my own predicate but it will add overhead and itś not that much code.
	// also the sub calling is different.
	for (AActor* const A: Actors) {
		// if the actor is in it, will execute the iterator. and if the iterator breaks. then break.
		if (IsValid(A) && InBox.IsInsideOrOn(A->GetActorLocation()) && Iterator.Execute(A, this)) return true;
	}

	for (AOTNode* const S: Nodes)
		if (IsValid(S) && S->IterateInside(Iterator, InBox)) return true; // bubble break

	return false; // continue the iteration
}

void AOTNode::Pack() {
	bool Can = true;
	for (int32 i = Nodes.Num()-1; i>=0;--i) {
		AOTNode* const N = Nodes[i];
		if (!N) {
			Nodes.RemoveAt(i, EAllowShrinking::No);
			continue;
		}

		N->Pack();
		Can = N->Nodes.Num() ==0;
	}

	if (!Can) return;

	int32 NumChilds=0;
	for (AOTNode* const N: Nodes) {
		NumChilds +=N->Actors.Num();
	}

	if (NumChilds>=ActorsMax) return;
	UE_LOG(LogJOctTree, Log, TEXT("%hs: %s: packing"), __func__, *GetNameSafe(this));
	for (AOTNode* const N: Nodes) { // this code is similar to tree::add but not quite
		Actors.Append(N->Actors);
		N->Actors.Empty(N->ActorsMax); // for the next time
		N->Return();
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

	const bool Updated = N->Update(Actor); // this checks for is valid
	if (Updated) RootNode->Pack();
	return Updated;
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

void AOctTree::Pack() {
	UE_LOG(LogJOctTree, Log, TEXT("%hs"), __func__);
	if (!RootNode) {
		UE_LOG(LogJOctTree, Warning, TEXT("%hs, could not get the root"), __func__);
		return;
	}

	RootNode->Pack();
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

void AOctTree::RebuildSameBox() {
	if (!RootNode) return; // TODO error
	Rebuild(RootNode->Box);
}

void AOctTree::Rebuild(const FBox& NewBox) {
	UE_LOG(LogJOctTree, Log, TEXT("%hs B=%s"), __func__, *NewBox.ToString());
	if (!RootNode) return; // TODO error
	if (!Pool) return;
	
	TArray<AOTNode*> Nodes;
	Nodes.Push(RootNode);
	RootNode = Cast<AOTNode>(Pool->Get());
	RootNode->SetBox(NewBox);

	while (Nodes.Num()>0) {
		AOTNode* const N = Nodes.Pop(EAllowShrinking::No);
		UE_LOG(LogJOctTree, Log, TEXT("%hs N=%s"), __func__, *GetNameSafe(N));
		if (!N) continue;
		UE_LOG(LogJOctTree, Log, TEXT("%hs N=%s An=%i"), __func__, *GetNameSafe(N), N->Actors.Num());

		for (AActor* const A: N->Actors) Add(A);
		N->Actors.Empty(ActorsMax);

		Nodes.Append(N->Nodes);
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

	Pool = Pooler->SetPool(1, AOTNode::StaticClass(), false, true, 10);
	RootNode = Cast<AOTNode>(Pool->Get());
	if (!RootNode) return;
	RootNode->SetUp(nullptr, ActorsMax);
}

void AOctTree::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (RootNode) RootNode->Return(true);
	RootNode = nullptr;

	if (Pool) Pool->Empty();
	Pool = nullptr;
	
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
		if (RootNode->IsInside(Actor)) return true;
		AOTNode* const NewRoot = Cast<AOTNode>(Pool->Get());
		if (!NewRoot) return false;

		const FBox& RBox = RootNode->Box;
		// find out which way we need to go
		const FVector Center = RBox.GetCenter();
		const FVector Ext = RBox.GetExtent();
		const FVector APos = Actor->GetActorLocation();
		const FVector Dir = APos - Center; // end-start
		const FVector Sign = Dir.GetSignVector();
		// const bool BDir[] = {Dir.X>=0, Dir.Y>=0, Dir.Z>=0}; // i could optimize with bit manip
		// this might work. or maybe is just nonsense
		const FVector ExtS = Ext*Sign;
		const FVector PCent = Center+ExtS;
		const FVector PExt = ExtS*2;
		const FVector PMax = PCent+PExt;
		FBox& PBox = NewRoot->Box; // alias
		PBox.Min = PCent-PExt;
		// reusing parboxmin.
		PBox.Max.X = FMath::Max(PBox.Min.X, PMax.X);
		PBox.Max.Y = FMath::Max(PBox.Min.Y, PMax.Y);
		PBox.Max.Z = FMath::Max(PBox.Min.Z, PMax.Z);
		// reusing parboxmax
		PBox.Min.X = FMath::Max(PBox.Min.X, PBox.Max.X);
		PBox.Min.Y = FMath::Max(PBox.Min.Y, PBox.Max.Y);
		PBox.Min.Z = FMath::Max(PBox.Min.Z, PBox.Max.Z);

		NewRoot->Split(); // avoid having to calculate the extend for the children based on the above node.
		// this is a hack might not work well
		for (AOTNode* N: NewRoot->Nodes) {
			if (!N) continue;
			if (!N->Box.IsInside(RBox)) continue;
			// the clone
			N->Nodes = RootNode->Nodes;
			N->Actors = RootNode->Actors;
			RootNode->Return(false); // we stole them
			break;
		}
	}

	return false;
}
// thas it?
