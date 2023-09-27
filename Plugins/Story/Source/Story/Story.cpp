// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "Story.h"

#include "Step.h"
#include "StoryTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogStory, Log, Log);

void UStory::Init_Implementation() {
}

void UStory::DeInit_Implementation() {
	Current = nullptr;
	Steps.Empty();
}

bool UStory::Start_Implementation(const FName& Name) {
	AStep ** const pStep = Steps.Find(Name);
	if (!pStep) {
		UE_LOG(LogStory, Warning, TEXT("Step could not be found. '%s'"), *Name.ToString());
		return false;
	}

	AStep* const Step = *pStep;
	if (!IsValid(Step)) {
		UE_LOG(LogStory, Warning, TEXT("Step was not valid. '%s'"), *Name.ToString());
		return false;
	}

	// stop the current step before starting a new one.
	Stop();// Is this a good idea?

	UE_LOG(LogStory, Log, TEXT("About to start step : '%s'"), *Name.ToString());
	Current = Step;
	Step->Start();
	OnStepStart.Broadcast(Step->Name);

	return true;
}

void UStory::Stop_Implementation(const FName& Name) {
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
	if (SeqStep>=Sequence.Num()) {
		OnSeqStop.Broadcast();
		return false;
	}

	return Start(Sequence[SeqStep]);
}

bool UStory::StartSequence_Implementation(const TArray<FName>& InSeq) {
	Sequence = InSeq;
	if (Sequence.IsEmpty()) return false;

	SeqStep = -1;
	return StartNextStep();
}
