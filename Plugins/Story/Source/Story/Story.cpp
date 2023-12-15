// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#include "Story.h"

#include "WorldPartition/DataLayer/DataLayerAsset.h"
#include "WorldPartition/DataLayer/DataLayerInstance.h"
#include "WorldPartition/DataLayer/DataLayerManager.h"

#include "Step.h"
#include "StoryTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogStory, Log, Log);

void UStory::Init() {}

void UStory::DeInit() {
	Current = nullptr;
	Steps.Empty();
}

UStory* UStory::Get(UWorld* W) {
	if (!IsValid(W)) return nullptr;

	UStory* const Story = W->GetSubsystem<UStory>();
	return IsValid(Story) ? Story : nullptr;
}


AStep* UStory::GetStep(const FName& Name) {
	AStep** const pStep = Steps.Find(Name);
	if (!pStep) {
		UE_LOG(LogStory, Warning, TEXT("Step could not be found. '%s'"), *Name.ToString());
		return nullptr;
	}

	AStep* const Step = *pStep;
	if (!IsValid(Step)) {
		UE_LOG(LogStory, Warning, TEXT("Step was not valid. '%s'"), *Name.ToString());
		return nullptr;
	}

	return Step;
}

bool UStory::StartNow(AStep* NewStep) {
	// stop the current step before starting a new one.
	Stop();

	Current = NewStep;
	if (!Current) return false;

	const bool Success = ToggleStepLayers();
	
	UE_LOG(LogStory, Log, TEXT("About to start step='%s' title='%s' DLLoadSuccess=%i"), 
		*Current->Name.ToString(), *Current->Title.ToString(), Success);

	// start before broadcasting start
	Current->Start();
	OnStart.Broadcast(Current);

	return true;
}

bool UStory::Start(const FName& Name) {
	// get the step
	AStep* const Step = GetStep(Name);
	if (!Step) return false;

	if (!Step->UseFade) {
		return StartNow(Step);
	}

	UWorld* const World = GetWorld();
	if (!IsValid(World)) return false;
	
	// do the fade
	OnFade.Broadcast(true, Step->Title);

	// callback
	auto l = [this, Step]() {
		// - call stop and start
		StartNow(Step);

		// now fade out
		// trigger this here. since the load layers is synchronous (on purpose)
		// so here it's the point where it "should"TM be loaded.
		auto l2 = [this]() {
			// do fade out
			OnFade.Broadcast(false, FText::GetEmpty());
		};

		UWorld* const World = GetWorld();
		if (!World) {
			UE_LOG(LogStory, Warning, TEXT("No world while attempted to fade out. i guess everything will be black."));
			return;
		}
		FTimerHandle H2;
		World->GetTimerManager().SetTimer(H2, l2, FadeTime+HoldTime, false);
	};

	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, l, FadeTime, false);

	return false;
}

void UStory::Stop() {
	if (!IsValid(Current)) return; // nothing to stop

	// clear up the Current variable so that the broadcast and startnextstep works fine.
	AStep* const Step = Current;
	Current = nullptr;
	
	UE_LOG(LogStory, Log, TEXT("About to stop step : '%s'"), *Step->Name.ToString());
	Step->Stop();

	// Call before starting the next step
	OnStop.Broadcast(Step);
}

void UStory::Add(AStep* Step) {
	if (!IsValid(Step)) return;
	// i think this replaces something if it already exists. and that's exactly what i want.
	Steps.Add(Step->Name, Step);
}

void UStory::Rem(const FName& Name) {
	// i think this doesn't crashes when it doesn't exists. if it does, change.
	Steps.Remove(Name);
}

const FName& UStory::GetCurrent() {
	const static FName Empty; // not NAME_None since i am returning a ref
	return IsValid(Current) ? Current->Name : Empty;
}

bool UStory::ToggleStepLayers() const {
	if (!Current) return false;

	bool Success = true;
	// FIRST load all DL and THEN unload
	// that way if there's an asset on both,
	// it will remain loaded instead of being temporarily unloaded and reloaded.
	// with all the possible issues it brings.
	for (const UDataLayerAsset* const DLA: Current->DL_Load) {
		Success = Success && ToggleDataLayer(DLA, true);
	}
	for (const UDataLayerAsset* const DLA: Current->DL_Unload) {
		Success = Success && ToggleDataLayer(DLA, false);
	}

	return Success;
}

bool UStory::ToggleDataLayer(const UDataLayerAsset* DLA, bool On) const{
	if (!IsValid(DLA)) return false;
	
	UE_LOG(LogStory, Log, TEXT("About to toggle data layer. load=%i name=%s"), On, *DLA->GetName());
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return false;
	
	const EDataLayerRuntimeState State = (On ? EDataLayerRuntimeState::Activated : EDataLayerRuntimeState::Unloaded);

	UDataLayerManager* const LayerManager = World->GetDataLayerManager();
	if (!IsValid(LayerManager)) {
		UE_LOG(LogStory, Warning, TEXT("Could not get the data layer manager"));
		return false;
	}

	const bool res = LayerManager->SetDataLayerRuntimeState(DLA, State, true);
	UE_LOG(LogStory, Log, TEXT("Data layer toggle. Ok=%i, load=%i, name='%s'"), res, On, *DLA->GetName());
	return res;
	// arigatou! https://kinnaji.com/2022/12/24/worldpartition-datalayer/
	
	/*  the subsystem  all is deprecated
	UDataLayerSubsystem* const Layers = World->GetSubsystem<UDataLayerSubsystem>();
	UDataLayerInstance* Instance = Layers->GetDataLayerInstanceFromAsset(DLA);
	Layers->SetDataLayerRuntimeState(Instance, State, true);
	if (!IsValid(Layers)) {
		UE_LOG(LogStory, Warning, TEXT("Could not get the data layer subsystem"), On, *DLA->GetName());
		return;
	}
	*/
}

bool UStory::StartNext(const FName& CurrentName) {
	// skip the check if there's no current. according to keikaku (no need to check if there's no one running)
	if (!CurrentName.IsNone() && IsValid(Current) && Current->Name != CurrentName) {
		UE_LOG(LogStory, Warning, TEXT("Attempted to stop a step that is not current!!! Current='%s' ToStop='%s'"),
			*Current->Name.ToString(), *CurrentName.ToString());
		return false;
	}
	
	++SeqStep;
	if (SeqStep >= Sequence.Num()) {
		Stop();
		OnSeqStop.Broadcast();
		return false;
	}

	return Start(Sequence[SeqStep]);
}

bool UStory::StartSequence(const TArray<FName>& InSeq) {
	Sequence = InSeq;
	SeqStep = -1;
	if (Sequence.IsEmpty()) return false;

	OnSeqStart.Broadcast();
	return StartNextStep();
}

void UStory::AutoFade(const FText& Title) {
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	OnFade.Broadcast(true, Title);
	
	auto l2 = [this]() {
		// do fade out
		OnFade.Broadcast(false, FText::GetEmpty());
	};

	FTimerHandle H2;
	FTimerDelegate TD2;
	TD2.BindLambda(l2);
	World->GetTimerManager().SetTimer(H2, TD2, FadeTime+HoldTime, false);
}
