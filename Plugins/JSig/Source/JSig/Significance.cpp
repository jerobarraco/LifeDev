// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "Significance.h"

#include "SignificanceManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogJSigSub, Log, Log);

USignificance::USignificance():Super() {}

USignificance* USignificance::Get(UWorld* W) {
	if (!IsValid(W)) return nullptr;
	USignificance* const Sig = W->GetSubsystem<USignificance>();
	return IsValid(Sig) ? Sig : nullptr;
}

void USignificance::Deinitialize() {
	Man = nullptr;
	PCs.Empty();
	Super::Deinitialize();
}

void USignificance::Reset() {
	UE_LOG(LogJSigSub, Log ,TEXT("%hs"), __func__);

	Man = nullptr;
	PCs.Empty();

	UWorld* const W = GetWorld();
	if (!W) return;

	Man = USignificanceManager::Get(W);
	int32 Num = NumPCs>0? NumPCs : W->GetNumPlayerControllers(); 
	for(FConstPlayerControllerIterator Iterator = W->GetPlayerControllerIterator(); Iterator && Num > 0; ++Iterator )
	{
		--Num;
		APlayerController* const PlayerController = Iterator->Get();
		if(!PlayerController) continue;
		PCs.Add(PlayerController);
	}

	DTAcum = 0;
}

void USignificance::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	Reset();
	// this won't make the animator work, but will make the USignificance get an EXTRA tick on a different interval (maybe the component's interval)
	// Animator->RegisterComponentWithWorld(GetWorld());
	// Animator->RegisterAllComponentTickFunctions(true);
	// Animator->RegisterComponent();
}

void USignificance::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	DTAcum += DeltaTime;
	if (DTAcum< TickInterval) return;
	DTAcum = 0;

	if (!Man || PCs.Num()==0) {
		// The manager is slow to get created, so we keep querying.
		Reset();
		return;
	}

	UE_LOG(LogJSigSub, Verbose, TEXT("%hs: will update"), __func__);

	TArray<FTransform> TransformArray;
	for (APlayerController* PC: PCs) {
		FVector ViewLocation;
		FRotator ViewRotation;
		PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

		// Viewpoints
		TransformArray.Emplace(ViewRotation, ViewLocation, FVector::OneVector);
		UE_LOG(LogJSigSub, Verbose,TEXT("%hs Got viewport %s-%s"), __func__, *ViewLocation.ToString(), *ViewRotation.ToString());
	}

	Man->Update(TArrayView<FTransform>(TransformArray));
}

// without this it will crash. yes. it will crash. https://forums.unrealengine.com/t/how-can-i-tick-a-tickableworldsubsystem/489697/3
TStatId USignificance::GetStatId() const {
	// https://benui.ca/unreal/tickable-object/
	// another way RETURN_QUICK_DECLARE_CYCLE_STAT( FMyTickableThing, STATGROUP_Tickables );
	return GetStatID();
}

UGameViewportClient* USignificance::GetAnyGameViewportClient() {
	if (GEngine->GameViewport)
	{
		return GEngine->GameViewport;
	}
	// Then Game viewport is attached to another world context than the main Engine one. (ie: PIE Net mode set to As Client)
	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for (const FWorldContext& Context : WorldContexts)
	{
		if ((Context.WorldType == EWorldType::PIE) && Context.World() != nullptr && Context.GameViewport != nullptr)
		{
			return Context.GameViewport;
		}
	}

	return nullptr;
}