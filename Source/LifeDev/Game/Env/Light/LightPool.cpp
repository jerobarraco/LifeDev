// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LightPool.h"

#include "Pool.h"

#include "Kismet/GameplayStatics.h"

#include "Interact/Animator/CRandomizer.h"
#include "Inventory/Flags.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Flashback/Flashback.h"

#include "LightItem.h"

// TODO maybe make a base pooler class. to avoid so much copypasta.

ALightPool::ALightPool():Super() {
	ItemClass = ALightItem::StaticClass();
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	bCollideWhenPlacing = false;
	
	Rnd = CreateDefaultSubobject<UCRandomizer>(TEXT("Rnd"));
	Rnd->DelayMin=2;
	Rnd->DelayMax=5;
	Rnd->IsLooping = true;
	Rnd->SetAutoActivate(false);
}

void ALightPool::SetActive(const bool Act) const {
	if (LIKELY(Rnd)) Rnd->SetActive(Act, false);
}

void ALightPool::Kill(const bool All) {
	UE_LOG(LogTemp, Log, TEXT("LightPool %hs All=%i"), __func__, All);

	SetActive(false);

	// return active to the pool
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ItemClass, Actors);
	for (AActor* const A: Actors) {
		// Hidden so it doesn't break the pooler.
		// IsValid to not stumble with the ones the pool might have killed.
		ALightItem* G = Cast<ALightItem>(A);
		if (UNLIKELY(!IsValid(G) || G->IsHidden())) continue;
		G->Return();
	}
}

void ALightPool::Init() {
	const UFlags* const Flags = UFlags::Instance(this);
	const float Alpha = Flags ? Flags->Get(LDConsts::Flags::Settings::Global::Foxy, -1) : .5;
	PoolSize = FMath::LerpStable(PoolSizeMin, PoolSizeMax, Alpha);
	UE_LOG(LogTemp, Log,
		TEXT("%hs Light PoolSize foxified. Min=%.4f, Max=%.4f, Res=%.4f, Foxy=%.4f"),
		__func__, PoolSizeMin, PoolSizeMax, PoolSize, Alpha);

	UFlashback* const Flashback = UFlashback::Instance(this);
	if (LIKELY(Flashback)) {
		FBTo(Flashback->GetValTo());
		// note this function is bound to OnTo and not OnChange.
		// it's more performant. does not choke the pooler system with requests and does not spams the log.
		Flashback->OnTo.AddUniqueDynamic(this, &ALightPool::FBTo);
	}

	ULSettings* const S = ULSettings::Instance(this);
	if (LIKELY(S))
		S->OnFeatUpdateEnviron.AddUniqueDynamic(this, &ALightPool::FeatUpdate);
	// FeatUpdate(EFeat::E_LightPOOL, S && S->GetFeat(EFeat::E_LightPOOL)); // TODO
}

void ALightPool::BeginPlay() {
	Super::BeginPlay();

	Pooler = UPooler::Instance(this);
	if (UNLIKELY(!Pooler)) {
		UE_LOG(LogTemp, Warning, TEXT("LightPool:%hs Can't get the pooler. this won't work. Stop"),
			__func__);
		return;
	}

	Rnd->OnTrigger.AddUniqueDynamic(this, &ALightPool::Spawn);
}

void ALightPool::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Kill(true); // ensure to kill all. if the GP dies we are killing the Lights too.

	if (LIKELY(Rnd)) Rnd->OnTrigger.RemoveAll(this);

	UFlashback* const Flashback = UFlashback::Instance(this);
	if (LIKELY(Flashback)) Flashback->OnTo.RemoveAll(this);
	
	ULSettings* const S = ULSettings::Instance(this);
	if (LIKELY(S)) S->OnFeatUpdateEnviron.RemoveAll(this);

	// set to max=0 to destroy them. set the trimtime to 0 to destroy now.
	if (Pooler)
		Pooler->SetPool(0, ItemClass, false, false, 0);
	Pooler = nullptr;
	
	Super::EndPlay(EndPlayReason);
}

void ALightPool::FeatUpdate(const EFeat Feat, const bool bEnabled) {
	// if (Feat != EFeat::E_LightPOOL) return; // TODO
	// SetActive(bEnabled);
}

void ALightPool::FBTo(const float To) {
	if (UNLIKELY(!Pooler)) return;

	const bool Active = To >= FBMin;
	SetActive(Active);

	// TODO Foxify the poolmax
	
	// update pool. depends on flashback, so done here.
	// the trim time will destroy items when not used.
	// controlling the Max will control the potential max amount of Lights in screen.
	const int32 MaxPre = FMath::TruncToInt(PoolSize*To);
	const int32 Max = Active ? MaxPre :0;
	// i wanted to have fun with branchless. but it's POSSIBLE the compiler would optimize this
	// const int32 Max = bitselect((int32) Active, MaxPre, 0);
	Pooler->SetPool(Max, ItemClass, false, false, TrimTime);
}

void ALightPool::Spawn() {
	if (UNLIKELY(!Pooler)) return;
	// this could return null on many situations. beware.
	Pooler->Get(ItemClass);
}
