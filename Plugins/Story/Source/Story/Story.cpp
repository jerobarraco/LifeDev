// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "Story.h"

#include <thread>

#include "ShaderPipelineCache.h"
#include "WorldPartition/DataLayer/DataLayerAsset.h"
#include "WorldPartition/DataLayer/DataLayerManager.h"

#include "Step.h"
#include "StoryTypes.h"
#include "JUtils/Misc/JUtilsMisc.h"

DEFINE_LOG_CATEGORY_STATIC(LogStory, Log, Log);

void UStory::Init() { }

void UStory::DeInit() {
	UDataLayerManager* const Manager = UDataLayerManager::GetDataLayerManager(this);
	if (LIKELY(IsValid(Manager))) Manager->OnDataLayerInstanceRuntimeStateChanged.RemoveAll(this);

	Current = nullptr;
	Steps.Empty();
}

UStory* UStory::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!IsValid(W))) return nullptr;

	UStory* const Story = W->GetSubsystem<UStory>();
	return IsValid(Story) ? Story : nullptr;
}

AStep* UStory::GetStep(const FName Name) {
	TObjectPtr<AStep>* const pStep = Steps.Find(Name);
	if (UNLIKELY(!pStep)) {
		UE_LOG(LogStory, Warning, TEXT("%hs Step could not be found. '%s'"), __func__, *Name.ToString());
		return nullptr;
	}

	AStep* const Step = *pStep;
	if (UNLIKELY(!IsValid(Step))) {
		UE_LOG(LogStory, Warning, TEXT("%hs Step was not valid. '%s'"), __func__, *Name.ToString());
		return nullptr;
	}

	return Step;
}

bool UStory::StartNow(AStep* const NewStep) {
	// notice we don't check here to allow stop to be called. this is by design.
	UE_LOG(LogStory, Log, TEXT("%hs -> %s"), __func__, *NewStep->Label.ToString());

	// stop the current step before starting a new one.
	Stop();

	// important to set before TryStart and ToggleStepLayers, so spawned objects can check for current step
	// allow to set nullptr
	Current = NewStep;
	if (UNLIKELY(!IsValid(Current))) {
		UE_LOG(LogStory, Log, TEXT("%hs -> Invalid step. Not starting."), __func__);
		return false;
	}

	const bool Success = ToggleStepLayers();
	UE_LOG(LogStory, Log, TEXT("%hs About to start step='%s' title='%s' DLLoadSuccess=%i"), 
		__func__, *Current->Label.ToString(), *Current->Title.ToString(), Success);

	// start before broadcasting start.
	// important so that it's actually ready for whom-ever is subscribed to the delegate
	// specially since some objects that want to react to this, might be on one of the loaded data layers.
	Current->TryStart();

	OnStart.Broadcast(Current);

	return true;
}

bool UStory::Start(const FName Name) {
	UE_LOG(LogStory, Log, TEXT("%hs: %s"), __func__, *Name.ToString());

	// get the step
	AStep* const Step = GetStep(Name);
	if (UNLIKELY(!IsValid(Step))) return false; // getstep prints warning

	// Note. i'm currently using the UseFade flag to also imply a blocking load.
	// it makes no difference as before because i don't need to change anything in the code.
	// it's just a notice to the user.
	// i did it that way instead of adding a variable because:
	// at first i didn't wanted to have redundant stuff, since it's very unlikely you'd want to block without a fade (it will get the frames stuck)
	// and it's very unlikely you'd want to not block during a fade. (or it wouldn't matter much).
	// then i've re-read the code, and noticed this. which pretty much implements such idea.
	// and makes the code much more simpler. so i'll keep it this way.
	if (!Step->UseFade)	return StartNow(Step);

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return false;

	// do the fade
	OnFade.Broadcast(false, Step->Title);

	// callback
	auto l = [this, Step]() {
		// - call stop and start
		// trigger this here. since the load layers is synchronous (on purpose)
		// so here it's the point where it "should"TM be loaded.
		// actually it seems that the dl is loaded, but the objects are not. so we wait below.
		StartNow(Step);

		auto l2 = [this]() {
			// i don't need to block the main thread. but does the gc flush and shader block work on the bg?
			AsyncTask(ENamedThreads::Type::AnyBackgroundThreadNormalTask, [this] {
			// attempt to wait for objects to be loaded.
			// notice there's a timer before this lambda, so we only block in case something still remains
			// blocking on load requires the data-tables to have "Override Block on slow streaming" to "Blocking"
			UE_LOG(LogStory, Log, TEXT("UStory::Start block start"));
			FStreamingManagerCollection& SMC = FStreamingManagerCollection::Get();
			SMC.BlockTillAllRequestsFinished(3.f, true); // still use time limit. i don't like soft-locks
			UE_LOG(LogStory, Log, TEXT("UStory::Start block end, flush start"));

			UWorld* const World3 = GetWorld(); // getting it again to avoid stale stuff.
			if (LIKELY(World3)) World3->FlushLevelStreaming(); // https://forums.unrealengine.com/t/blocking-load-not-working-when-streaming-levels/368085/25?u=nande
			UE_LOG(LogStory, Log, TEXT("UStory::Flush end"));

			// there's a way to get the load percent for a specific package https://forums.unrealengine.com/t/ue5-5-5-6-call-function-getasyncloadpercentage-packagename-the-return-value-is-incorrect-return-1/2652655?u=nande

			// this will skip GC while async loading, unless you set on your config
			// CVarPerformGCWhileAsyncLoading gc.PerformGCWhileAsyncLoading
			// hopefully the above things would wait until all asyncs are done.
			// calling this here will crash the engine, due to a check. that it can't happen during "tick".
			// if (LIKELY(GEngine)) GEngine->PerformGarbageCollectionAndCleanupActors();
			// this won't run gc, but will schedule it for the next time.
			// hopefully that will happen next frame. which means the fade might hitch.
			// hopefully not noticeably
			if (UseFadeGC & LIKELY(GEngine)) {
				UE_LOG(LogStory, Log, TEXT("UStory::Scheduled GC"));
				GEngine->ForceGarbageCollection(true);
			}

				// attempt at waiting for shaders to compile on load.
				while (FShaderPipelineCache::NumPrecompilesRemaining()>0) { // is it ok to spinlock this thread? should i try a different one?
					UE_LOG(LogStory, Log, TEXT("UStory::Waiting on shaders. %i"), FShaderPipelineCache::NumPrecompilesRemaining());
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
				}
				AsyncTask(ENamedThreads::Type::GameThread, [this] {
					// do fade out
					OnFade.Broadcast(true, FText::GetEmpty());
				});
			});
		};

		const UWorld* const World2 = GetWorld(); // getting it again to avoid stale stuff.
		if (UNLIKELY(!World2)) {
			UE_LOG(LogStory, Warning, TEXT("%hs No world while attempted to fade out."
				" i guess everything will be black."), __func__);
			return;
		}
		FTimerHandle H2;
		World2->GetTimerManager().SetTimer(H2, l2, HoldTime, false);
	};

	FTimerHandle H;
	World->GetTimerManager().SetTimer(H, l, FadeTime, false);

	return true;
}

void UStory::Stop() {
	if (UNLIKELY(!IsValid(Current))) {
		UE_LOG(LogStory, Log, TEXT("%hs -> Nothing to stop. Skip."), __func__);
		return; // nothing to stop
	}
	UE_LOG(LogStory, Log, TEXT("%hs -> %s"), __func__, *Current->Label.ToString());

	// clear up the Current variable so that the broadcast and startnextstep works fine.
	AStep* const Step = Current;
	Current = nullptr;
	
	Step->Stop();

	// Call before starting the next step
	OnStop.Broadcast(Step);
}

void UStory::Add(AStep* const Step) {
	if (UNLIKELY(!IsValid(Step))) return;

	// i think this replaces something if it already exists. and that's exactly what i want.
	Steps.Add(Step->Label, Step);
}

void UStory::Rem(const FName Name) {
	// i think this doesn't crash when it doesn't exist. if it does, change.
	Steps.Remove(Name);
}

FName UStory::GetCurrent() const {
	// const static FName Empty = NAME_None;
	return LIKELY(IsValid(Current)) ? Current->Label : NAME_None;
}

bool UStory::ToggleStepLayers() const {
	if (UNLIKELY(!IsValid(Current))) return false;

	UE_LOG(LogStory, Log, TEXT("%hs -> %s"), __func__, *Current->Label.ToString());

	bool Success = true;

	// FIRST load all DL and THEN unload
	// that way if there's an asset on two DL (one being loaded and another unloaded),
	// it will remain loaded instead of being temporarily unloaded and reloaded.
	// with all the possible issues it brings.
	for (const TSoftObjectPtr<UDataLayerAsset>& SLA: Current->DL_Load) {
		const UDataLayerAsset* const DLA = SLA.LoadSynchronous();
		if (UNLIKELY(!IsValid(DLA))) continue;

		const bool CurSuccess = ToggleLayer(DLA, true);
		Success = Success & CurSuccess; // like this to avoid short circuit
		Current->DL_Unload.RemoveSwap(SLA); // avoid possible issue of someone adding the same layer to unload as well
	}

	for (const TSoftObjectPtr<UDataLayerAsset>& SLA: Current->DL_Unload) {
		const UDataLayerAsset* const DLA = SLA.LoadSynchronous();
		if (UNLIKELY(!IsValid(DLA))) continue;

		const bool CurSuccess = ToggleLayer(DLA, false);
		Success = Success & CurSuccess; // like this to avoid short circuit
	}

	return Success;
}

bool UStory::ToggleLayer(const UDataLayerAsset* const DLA, bool On) const {
	if (UNLIKELY(!IsValid(DLA))) return false;

	const bool Success = UJUtilsMisc::ToggleDataLayer(this, DLA, On);
	UE_LOG(LogStory, Log, TEXT("%hs Data layer toggle. Ok=%i, load=%i, name='%s'"),
		__func__, Success, On, *DLA->GetName());
	// On loaded is usually better AFTER, on unloaded is usually better before. Which one is better? Time will tell.
	OnDlChange.Broadcast(DLA->GetFName(), On, Success);

	return Success;
}

bool UStory::StartNext(const FName CurrentName) {
	UE_LOG(LogStory, Log, TEXT("%hs. CurrentName=%s"), __func__, *CurrentName.ToString());

	// skip the check if there's no current. according to keikaku (no need to check if there's no one running)
	if (UNLIKELY(!CurrentName.IsNone() & (IsValid(Current) && Current->Label != CurrentName))) {
		UE_LOG(LogStory, Warning, TEXT("%hs Attempted to stop a step that is not current!!! Current='%s' ToStop='%s'"),
			__func__, *Current->Label.ToString(), *CurrentName.ToString());
		return false;
	}
	
	++SeqStep;
	if (UNLIKELY(SeqStep >= Sequence.Num())) {
		UE_LOG(LogStory, Log, TEXT("%hs Reached end of sequence. Stopping"), __func__);
		// not stopping here to allow transitions between end of chapter to flow correctly (i.e. fade and load)
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
	UE_LOG(LogStory, Log, TEXT("%hs: title=%s"), __func__, *Title.ToString());

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	OnFade.Broadcast(false, Title);

	auto l2 = [this]() {
		// see Start()
		if (UseFadeGC & LIKELY(GEngine)) {
			UE_LOG(LogStory, Log, TEXT("UStory::Scheduled GC"));
			GEngine->ForceGarbageCollection(true);
		}
		// do fade out
		OnFade.Broadcast(true, FText::GetEmpty());
	};

	FTimerHandle H2;
	FTimerDelegate TD2;
	TD2.BindLambda(l2);
	World->GetTimerManager().SetTimer(H2, TD2, FadeTime+HoldTime, false);
}
