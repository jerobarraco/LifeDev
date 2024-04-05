// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "Significance.h"

#include "SignificanceManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogJSigSub, Log, Log);

USignificance::USignificance():Super() {}

USignificance* USignificance::Instance(UObject* O) {
	if (!IsValid(O)) return nullptr;
	UWorld* const W = O->GetWorld();
	if (!IsValid(W)) return nullptr;
	USignificance* const Sig = W->GetSubsystem<USignificance>();
	return IsValid(Sig) ? Sig : nullptr;
}

void USignificance::Deinitialize() {
	UE_LOG(LogJSigSub, Log, TEXT("%hs"), __func__);
	Man = nullptr;
	PCs.Empty();
	Super::Deinitialize();
}

void USignificance::Reset() {
	UE_LOG(LogJSigSub, Verbose, TEXT("%hs"), __func__); // verbose since it can be triggered every frame

	Man = nullptr;
	PCs.Empty();

	UWorld* const W = GetWorld();
	if (!W) return;

	Man = USignificanceManager::Get(W);
	int32 Num = NumPCs>0? NumPCs : W->GetNumPlayerControllers(); 
	for(FConstPlayerControllerIterator Iterator = W->GetPlayerControllerIterator();
		Iterator && Num > 0; ++Iterator ) {
		--Num;
		APlayerController* const PlayerController = Iterator->Get();
		if(!PlayerController) continue;
		PCs.Add(PlayerController);
	}

	DTAcum = 0;
}

void USignificance::Initialize(FSubsystemCollectionBase& Collection) {
	UE_LOG(LogJSigSub, Log, TEXT("%hs useBgThread=%i numPCs=%i interval=%5.3f tickWhenPaused=%i"),
		__func__, UseBGThread, NumPCs, TickInterval, TickWhenPaused);
	Super::Initialize(Collection);
	Reset();
}

void USignificance::DoTick() {
	if (!Man || PCs.Num()==0) {
		UE_LOG(LogJSigSub, Log, TEXT("%hs. Force Reset."), __func__);
		// The manager is slow to get created, so we keep querying.
		Reset();
		return; // return in case the above check still fails
	}

	UE_LOG(LogJSigSub, Verbose, TEXT("%hs: will update"), __func__);

	TArray<FTransform> TransformArray;
	for (APlayerController* const PC: PCs) {
		FVector ViewLocation;
		FRotator ViewRotation;
		PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

		// Viewpoints
		TransformArray.Emplace(ViewRotation, ViewLocation, FVector::OneVector);
		UE_LOG(LogJSigSub, Verbose, TEXT("%hs Got viewpoint %s-%s"), __func__, *ViewLocation.ToString(), *ViewRotation.ToString());
	}

	Man->Update(TArrayView<FTransform>(TransformArray));
}

void USignificance::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	DTAcum += DeltaTime;
	if (DTAcum< TickInterval) return;
	DTAcum = 0;

	UE_LOG(LogJSigSub, Verbose, TEXT("%hs"), __func__);

	if (UseBGThread) {
		// if i make it static and the subsystem gets recreated. this might still keep a ref to the old "this"
		TUniqueFunction<void()> F = [this] {
			this->DoTick();
		};
		// this thread works on android. "BackgroundThreadPriority" will NOT execute.
		AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, MoveTemp(F));
	} else {
		DoTick();
	}
}

// without this it will crash. yes. it will crash. https://forums.unrealengine.com/t/how-can-i-tick-a-tickableworldsubsystem/489697/3
TStatId USignificance::GetStatId() const {
	// https://benui.ca/unreal/tickable-object/
	// another way RETURN_QUICK_DECLARE_CYCLE_STAT( FMyTickableThing, STATGROUP_Tickables );
	// RETURN_QUICK_DECLARE_CYCLE_STAT(USignificance, STATGROUP_Tickables);
	return GetStatID();
}

UGameViewportClient* USignificance::GetAnyGameViewportClient() {
	if (GEngine && GEngine->GameViewport) return GEngine->GameViewport;
	
	// Then Game viewport is attached to another world context than the main Engine one. (ie: PIE Net mode set to As Client)
	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for (const FWorldContext& Context : WorldContexts)
	{
		if ((Context.WorldType == EWorldType::PIE) && Context.World() && Context.GameViewport)
		{
			return Context.GameViewport;
		}
	}

	return nullptr;
}
