// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "GhostPool.h"

#include "Pool.h"

#include "Kismet/GameplayStatics.h"

#include "Interact/Animator/CRandomizer.h"
#include "Inventory/Flags.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "GhostItem.h"

AGhostPool::AGhostPool():Super() {
	ItemClass = AGhostItem::StaticClass();
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	bCollideWhenPlacing = false;
	
	USceneComponent* const Cmp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Cmp);

	Rnd = CreateDefaultSubobject<UCRandomizer>(TEXT("Rnd"));
	Rnd->DelayMin=2;
	Rnd->DelayMax=5;
	Rnd->IsLooping = true;
	Rnd->SetAutoActivate(false);
}

void AGhostPool::SetActive(const bool Act) const {
	Rnd->SetActive(Act);
}

void AGhostPool::Kill(const bool All) {
	UE_LOG(LogTemp, Log, TEXT("GhostPool %hs All=%i"), __func__, All);

	SetActive(false);
	
	// i could return them to the pool. but honestly. it's not that big of a deal.
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ItemClass, Actors);
	if (!All) {
		for (AActor* const A: Actors) {
			// Hidden so it doesn't break the pooler.
			// IsValid to not stumble with the ones the pool might have killed.
			AGhostItem* G = Cast<AGhostItem>(A);
			if (!IsValid(G) || G->IsHidden()) continue;
			G->Return();
		}
		return;
	}

	if (Pooler)
		// set to max=0 to destroy them. set the trimtime to 0 to destroy now.
		Pooler->SetPool(0, ItemClass, false, false, 0);
	// kill the rest
	for (AActor* const A: Actors) {
		// Hidden so it doesn't break the pooler.
		// IsValid to not stumble with the ones the pool might have killed.
		if (!IsValid(A) || A->IsHidden()) continue;
		A->Destroy();
	}
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
	if (Flashback) {
		FBTo(Flashback->GetValTo());
		Flashback->OnTo.AddUniqueDynamic(this, &AGhostPool::FBTo);
	}

	const UFlags* const Flags = UFlags::Instance(this);
	const float Alpha = Flags ? Flags->Get(LDConsts::Flags::Settings::Global::Foxy, -1) : .5;
	PoolSize = FMath::LerpStable(PoolSizeMin, PoolSizeMax, Alpha);
	UE_LOG(LogTemp, Log,
		TEXT("%hs Ghost PoolSize foxified. Min=%.4f, Max=%.4f, Res=%.4f, Foxy=%.4f"),
		__func__, PoolSizeMin, PoolSizeMax, PoolSize, Alpha);
}

void AGhostPool::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Kill(true); // ensure to kill all. if the GP dies we are killing the ghosts too.
	Rnd->OnTrigger.Clear();
	UFlashback* const Flashback = UFlashback::Instance(this);
	if (Flashback)
		Flashback->OnTo.RemoveAll(this);
	
	Super::EndPlay(EndPlayReason);
}

void AGhostPool::FBTo(const float To) {
	if (UNLIKELY(!Pooler)) return;

	const bool Active = To >= FBMin;
	SetActive(Active);

	// TODO Foxify the poolmax

	// update pool
	// the trim time will destroy items when not used.
	const int32 MaxPre = FMath::TruncToInt(PoolSize* To);
	const int32 Max = Active ? MaxPre :0;
	// // i wanted to have fun with branchless. but it's POSSIBLE the compiler would optimize this
	// const int32 Max = bitselect((int32) Active, MaxPre, 0);
	Pooler->SetPool(Max, ItemClass, false, false, TrimTime);
}

void AGhostPool::Spawn() {
	if (UNLIKELY(!Pooler)) return;
	// this could return null on many situations. beware.
	Pooler->Get(ItemClass);
}
