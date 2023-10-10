// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "Story.h"

#include "Step.h"
#include "StoryTypes.h"
#include "WorldPartition/DataLayer/WorldDataLayers.h"

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

void UStory::ToggleStepLayers() {
	for (const FName& Name: Current->DL_Unload) {
		ToggleDataLayer(Name, false);
	}
	for (const FName& Name: Current->DL_Load) {
		ToggleDataLayer(Name, true);
	}
}

bool UStory::StartNow(AStep* NewStep) {
	// stop the current step before starting a new one.
	Stop();

	Current = NewStep;
	if (!Current) return false;

	ToggleStepLayers();
	
	UE_LOG(LogStory, Log, TEXT("About to start step='%s' title='%s'"), 
		*Current->Name.ToString(), *Current->Title.ToString());
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
	};

	FTimerHandle H;
	FTimerDelegate TD;
	TD.BindLambda(l);
	World->GetTimerManager().SetTimer(H, TD, FadeTime, false);

	auto l2 = [this]() {
		// do fade out
		OnFade.Broadcast(false, FText::GetEmpty());
	};

	FTimerHandle H2;
	FTimerDelegate TD2;
	TD2.BindLambda(l2);
	World->GetTimerManager().SetTimer(H2, TD2, FadeTime+HoldTime, false);

	return false;
}

void UStory::Stop(const FName& Name) {
	if (!IsValid(Current)) return; // nothing to stop

	if (!Name.IsNone() && Current->Name != Name) {
		UE_LOG(LogStory, Warning, TEXT("Attempted to stop a step that is not current!!! Current='%s' ToStop='%s'"),
			*Current->Name.ToString(), *Name.ToString());
		return;
	}

	// clear up the Current variable so that the broadcast and startnextstep works fine.
	AStep* const Step = Current;
	Current = nullptr;
	
	UE_LOG(LogStory, Log, TEXT("About to stop step : '%s'"), *Step->Name.ToString());
	Step->Stop();
	// Call before starting the next step
	OnStop.Broadcast(Step);
	
	StartNextStep();
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

void UStory::ToggleDataLayer(const FName& Name, bool On) {
	UE_LOG(LogStory, Log, TEXT("About to toggle data layer. load=%i name=%s"), On, *Name.ToString());
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	
	AWorldDataLayers* const Layers = World->GetWorldDataLayers();
	if (!IsValid(Layers)) return;

	const EDataLayerRuntimeState State = (On ? EDataLayerRuntimeState::Activated : EDataLayerRuntimeState::Unloaded);
	const UDataLayerInstance* const Instance = Layers->GetDataLayerInstance(Name);
	if (!Instance) {
		UE_LOG(LogStory, Warning, TEXT("Colud not get data layer instance. Name='%s'"), *Name.ToString());
		return;
	}

	Layers->SetDataLayerRuntimeState(Instance, State, true);
}

bool UStory::StartNextStep() {
	++SeqStep;
	if (SeqStep >= Sequence.Num()) {
		OnSeqStop.Broadcast();
		return false;
	}

	return Start(Sequence[SeqStep]);
}

bool UStory::StartSequence(const TArray<FName>& InSeq) {
	Sequence = InSeq;
	SeqStep = -1;
	if (Sequence.IsEmpty()) return false;
	// TODO add on SeqStart?
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
