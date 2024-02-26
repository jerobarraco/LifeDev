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

UStory* UStory::Instance(UWorld* W) {
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
	// notice we don't check here to allow stop to be called. this is by design.
	UE_LOG(LogStory, Log, TEXT("%hs -> %s"), __func__, *NewStep->Name.ToString());

	// stop the current step before starting a new one.
	Stop();

	Current = NewStep;
	if (!IsValid(Current)) {
		UE_LOG(LogStory, Log, TEXT("%hs -> Invalid step. Not starting."));
		return false;
	}

	const bool Success = ToggleStepLayers();
	UE_LOG(LogStory, Log, TEXT("About to start step='%s' title='%s' DLLoadSuccess=%i"), 
		*Current->Name.ToString(), *Current->Title.ToString(), Success);

	// start before broadcasting start
	Current->Start();
	OnStart.Broadcast(Current);

	return true;
}

bool UStory::Start(const FName& Name) {
	UE_LOG(LogStory, Log, TEXT("%hs: %s"), __func__, *Name.ToString());

	// get the step
	AStep* const Step = GetStep(Name);
	if (!IsValid(Step)) return false; // getstep prints warning

	if (!Step->UseFade)
		return StartNow(Step);

	UWorld* const World = GetWorld();
	if (!IsValid(World)) return false;
	
	// do the fade
	OnFade.Broadcast(false, Step->Title);

	// callback
	auto l = [this, Step]() {
		// - call stop and start
		StartNow(Step);

		// now fade in
		// trigger this here. since the load layers is synchronous (on purpose)
		// so here it's the point where it "should"TM be loaded.
		auto l2 = [this]() {
			// do fade out
			OnFade.Broadcast(true, FText::GetEmpty());
		};

		UWorld* const World = GetWorld();
		if (!World) {
			UE_LOG(LogStory, Warning, TEXT("No world while attempted to fade out. i guess everything will be black."));
			return;
		}
		FTimerHandle H2;
		World->GetTimerManager().SetTimer(H2, l2, HoldTime, false);
	};

	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, l, FadeTime, false);

	return false;
}

void UStory::Stop() {
	if (!IsValid(Current)) return; // nothing to stop
	UE_LOG(LogStory, Log, TEXT("%hs -> %s"), __func__, *Current->Name.ToString());

	// clear up the Current variable so that the broadcast and startnextstep works fine.
	AStep* const Step = Current;
	Current = nullptr;
	
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
	if (!IsValid(Current)) return false;

	UE_LOG(LogStory, Log, TEXT("%hs -> %s"), __func__, *Current->Name.ToString());

	bool Success = true;
	// FIRST load all DL and THEN unload
	// that way if there's an asset on two DL (one being loaded and another unloaded),
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

bool UStory::ToggleDataLayer(const UDataLayerAsset* DLA, bool On) const {
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

	const bool Success = LayerManager->SetDataLayerRuntimeState(DLA, State, false);
	UE_LOG(LogStory, Log, TEXT("Data layer toggle. Ok=%i, load=%i, name='%s'"), Success, On, *DLA->GetName());
	// On loaded is usually better AFTER, on unloaded is usually better before. Which one is better? Time will tell.
	OnDlChange.Broadcast(DLA->GetFName(), On, Success);

	return Success;
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
	UE_LOG(LogStory, Log, TEXT("%hs. CurrentName=%s"), __func__, *CurrentName.ToString());

	// skip the check if there's no current. according to keikaku (no need to check if there's no one running)
	if (!CurrentName.IsNone() && IsValid(Current) && Current->Name != CurrentName) {
		UE_LOG(LogStory, Warning, TEXT("Attempted to stop a step that is not current!!! Current='%s' ToStop='%s'"),
			*Current->Name.ToString(), *CurrentName.ToString());
		return false;
	}
	
	++SeqStep;
	if (SeqStep >= Sequence.Num()) {
		UE_LOG(LogStory, Log, TEXT("Reached end of sequence. Stopping"));
		// not stopping here to allow transitions between end of chapter to flow correctly (i.e. fade)
		// OnSeqStop will tell the gamemode that the sequence (chapter) finished,
		// the GM will load the next chapter, and call StartSequence.
		// StartSequence will call this, which calls Start, and since Current is still valid,
		// it will transition correctly.
		// Stop();
		OnSeqStop.Broadcast();
		return false;
	}

	return Start(Sequence[SeqStep]);
}

bool UStory::StartSequence(const TArray<FName>& InSeq) {
	UE_LOG(LogStory, Log, TEXT("%hs: num=%i"), __func__, InSeq.Num());
	Sequence = InSeq;
	SeqStep = -1;
	if (Sequence.IsEmpty()) return false;

	// this triggers before the sequence is actually started. which is inconvenient for the client,
	// but makes sense in the code. to avoid potential issues, and keeps the order of signals.
	// StartNext could call OnSeqStop
	OnSeqStart.Broadcast();
	// Important to call startNext to allow for appropriate transitions between sequences (chapters)
	return StartNext();
}

void UStory::AutoFade(const FText& Title) {
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	OnFade.Broadcast(false, Title);
	
	auto l2 = [this]() {
		// do fade out
		OnFade.Broadcast(true, FText::GetEmpty());
	};

	FTimerHandle H2;
	FTimerDelegate TD2;
	TD2.BindLambda(l2);
	World->GetTimerManager().SetTimer(H2, TD2, FadeTime+HoldTime, false);
}
