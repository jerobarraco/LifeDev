// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "JSig.h"

#include "SignificanceManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogJSigSub, Log, Log);

UJSig::UJSig():Super() {}

UJSig* UJSig::Instance(const UObject* O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!IsValid(W))) return nullptr;

	UJSig* const Sig = W->GetSubsystem<UJSig>();
	return IsValid(Sig) ? Sig : nullptr;
}

void UJSig::Deinitialize() {
	UE_LOG(LogJSigSub, Log, TEXT("%hs"), __func__);
	Man = nullptr;
	PCs.Empty();
	Super::Deinitialize();
}

void UJSig::Reset() {
	UE_LOG(LogJSigSub, Verbose, TEXT("%hs"), __func__); // verbose since it can be triggered every frame

	Man = nullptr;
	PCs.Empty();

	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	Man = USignificanceManager::Get(W);
	int32 Num = NumPCs>0? NumPCs : W->GetNumPlayerControllers(); 
	for(FConstPlayerControllerIterator Iterator = W->GetPlayerControllerIterator();
		Iterator && Num > 0; ++Iterator ) {
		--Num;
		APlayerController* const Controller = Iterator->Get();
		if(UNLIKELY(!Controller)) continue;

		PCs.AddUnique(Controller);
	}

	DTAcum = 0;
}

void UJSig::Initialize(FSubsystemCollectionBase& Collection) {
	UE_LOG(LogJSigSub, Log, TEXT("%hs useBgThread=%i numPCs=%i interval=%5.3f tickWhenPaused=%i"),
		__func__, UseBGThread, NumPCs, TickInterval, TickWhenPaused);
	Super::Initialize(Collection);
	Reset();
}

void UJSig::DoTick() {
	if (UNLIKELY(!IsValid(Man) || PCs.Num()==0)) {
		// Verbose since this could trigger a lot, or never stop.
		UE_LOG(LogJSigSub, Verbose, TEXT("%hs. Force Reset."), __func__);
		// The manager is slow to get created, so we keep querying.
		Reset();
		return; // return in case the above check still fails
	}

	UE_LOG(LogJSigSub, Verbose, TEXT("%hs: will update"), __func__);

	TArray<FTransform> TransformArray;
	for (APlayerController* const PC: PCs) {
		if (UNLIKELY(!IsValid(PC))) continue;
		FVector ViewLocation;
		FRotator ViewRotation;
		PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

		// Viewpoints
		TransformArray.Emplace(ViewRotation, ViewLocation, FVector::OneVector);
		UE_LOG(LogJSigSub, Verbose, TEXT("%hs Got viewpoint %s-%s"), __func__, *ViewLocation.ToString(), *ViewRotation.ToString());
	}

	Man->Update(TArrayView<FTransform>(TransformArray));
}

void UJSig::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);

	DTAcum += DeltaTime;
	if (UNLIKELY(DTAcum< TickInterval)) return;
	DTAcum = 0;

	UE_LOG(LogJSigSub, Verbose, TEXT("%hs"), __func__);

	if (UseBGThread) {
		// if i make it static and the subsystem gets recreated. this might still keep a ref to the old "this"
		TUniqueFunction<void()> F = [this] {
			this->DoTick();
		};
		// this thread works on android. "BackgroundThreadPriority" will NOT execute.
		// i wonder what would happen if tick is run on every frame. could this possibly aggregate and run multiple times on a frame?
		AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, MoveTemp(F));
	} else
		DoTick();
}

// without this it will crash. yes. it will crash. https://forums.unrealengine.com/t/how-can-i-tick-a-tickableworldsubsystem/489697/3
TStatId UJSig::GetStatId() const {
	// https://benui.ca/unreal/tickable-object/
	// another way RETURN_QUICK_DECLARE_CYCLE_STAT( FMyTickableThing, STATGROUP_Tickables );
	// RETURN_QUICK_DECLARE_CYCLE_STAT(USignificance, STATGROUP_Tickables);
	return GetStatID();
}
