// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "GhostPool.h"

#include "GhostItem.h"
#include "Pool.h"
#include "Interact/Animator/CRandomizer.h"
#include "LifeDev/Game/Flashback/Flashback.h"

AGhostPool::AGhostPool():Super() {
	ItemClass = AGhostItem::StaticClass();
	
	USceneComponent* const Cmp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Cmp);

	Rnd = CreateDefaultSubobject<UCRandomizer>(TEXT("Rnd"));
	Rnd->DelayMin=2;
	Rnd->DelayMax=5;
	Rnd->IsLooping = true;
	Rnd->SetAutoActivate(false);
}

void AGhostPool::SetActive(bool Act) {
	Rnd->SetActive(Act);
}

void AGhostPool::BeginPlay() {
	Super::BeginPlay();
	
	Pooler = UPooler::Instance(this);
	if (!Pooler) {
		UE_LOG(LogTemp, Warning, TEXT("GhostPool:%hs Can't get the pooler. this won't work. Stop"),
			__func__);
		return;
	}

	Rnd->OnTrigger.AddUniqueDynamic(this, &AGhostPool::Spawn);
	UFlashback* const Flashback = UFlashback::Instance(this);
	if (Flashback)
		Flashback->OnTo.AddUniqueDynamic(this, &AGhostPool::FBTo);
}

void AGhostPool::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Rnd->OnTrigger.Clear();
	UFlashback* const Flashback = UFlashback::Instance(this);
	if (Flashback)
		Flashback->OnTo.RemoveAll(this);
	
	Super::EndPlay(EndPlayReason);
}
void AGhostPool::FBTo(const float To) {
	if (!Pooler) return;

	const bool Active = To >= FBMin;
	SetActive(Active);

	// update pool
	// the trim time will destroy items when not used.
	const int32 MaxPre = FMath::TruncToInt(PoolMax* To);
	const int32 Max = Active ? MaxPre :0;
	// // i wanted to have fun with branchless. but it's POSSIBLE the compiler would optimize this 
	// const int32 Max = bitselect((int32) Active, MaxPre, 0);
	Pooler->AddPool(Max, ItemClass, false, false, TrimTime);
}

void AGhostPool::Spawn() {
	if (!Pooler) return;
	// this could return null on many situations. beware.
	Pooler->Get(ItemClass);
}


/*
inline static int bitselect(int condition, int truereturnvalue, int falsereturnvalue)
{
	return (truereturnvalue & -condition) | (falsereturnvalue & ~(-condition)); //a when TRUE and b when FALSE
}

inline static float bitselect(int condition, float truereturnvalue, float falsereturnvalue)
{
	//Reinterpret floats. Would work because it's just a bit select, no matter the actual value
	int& at = reinterpret_cast<int&>(truereturnvalue);
	int& af = reinterpret_cast<int&>(falsereturnvalue);
	int res = (at & -condition) | (af & ~(-condition)); //a when TRUE and b when FALSE
	return  reinterpret_cast<float&>(res);
}
https://stackoverflow.com/a/30084391

*/