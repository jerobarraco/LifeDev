// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "Story.h"

#include "Step.h"
#include "StoryTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogStory, Log, Log);

void UStory::Init() {}

void UStory::DeInit() {
	Current = nullptr;
	Steps.Empty();
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

bool UStory::Start(const FName& Name) {
	AStep* const Step = GetStep(Name);
	if (!Step) return false;
	
	// TODO if step has fade do fade here
	// stop the current step before starting a new one.
	Stop();// Is this a good idea?

	UE_LOG(LogStory, Log, TEXT("About to start step : '%s'"), *Name.ToString());
	Current = Step;
	Step->Start();
	OnStepStart.Broadcast(Step->Name);

	return true;
}

bool UStory::Start2(const FName& Name) {
	// get the step
	AStep* const Step = GetStep(Name);
	if (!Step) return false;

	if (!Step->UseFade) {
		// TODO change this function to receive an AStep instead of name and rename it
		return Start(Name);
	}

	// do the fade
	OnFade.Broadcast(true, Step->Title);
	
	// callback
	auto l = [this, Name]() {
		// - call stop and start
		Start(Name);
		// do fade out
		OnFade.Broadcast(false, FText::GetEmpty());
	};

	FTimerHandle H;
	FTimerDelegate TD;
	TD.BindLambda(l);
	GetWorld()->GetTimerManager().SetTimer(H, TD, FadeTime, false);

	return false;
}

void UStory::Stop(const FName& Name) {
	if (!IsValid(Current)) return; // nothing to stop

	if (!Name.IsNone() && Current->Name != Name) {
		UE_LOG(LogStory, Warning, TEXT("Attempted to stop a step that is not current!!! Current='%s' ToStop='%s'"),
			*Current->Name.ToString(), *Name.ToString());
		return;
	}

	AStep* const Step = Current;
	UE_LOG(LogStory, Log, TEXT("About to stop step : '%s'"), *Step->Name.ToString());
	// done before calling stop to allow for other functions to call this.
	Current = nullptr;
	Step->Stop();
	OnStepStop.Broadcast(Step->Name);
	
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
	static FName Empty;
	return IsValid(Current) ? Current->Name : Empty;
}

bool UStory::StartNextStep() {
	++SeqStep;
	if (SeqStep >= Sequence.Num()) {
		OnSeqStop.Broadcast();
		return false;
	}

	// TODO call Start2
	return Start(Sequence[SeqStep]);
	return Start2(Sequence[SeqStep]);
}

bool UStory::StartSequence(const TArray<FName>& InSeq) {
	Sequence = InSeq;
	SeqStep = -1;
	Stop();
	if (Sequence.IsEmpty()) return false;

	return StartNextStep();
}

// TODO:
//	Leave this functions as they are
//  Create new functions for the handling of the fade. that call these functions.
// Later on rename stuff and call to the new ones instead
// TODO slowly port over stuff from the game mode to here.
