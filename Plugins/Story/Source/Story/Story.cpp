// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "Story.h"

#include "Step.h"
#include "StoryTypes.h"

#include "Kismet/GameplayStatics.h"

#pragma optimize("", off)

void UStory::Init_Implementation() {
	TArray<AActor*> Actors;
	// TODO find another way, they could be unloaded
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStep::StaticClass(), Actors);
	Steps.Empty();
	for (int32 i= 0; i<Actors.Num(); ++i) {
		AActor* Actor = Actors[i];
		if (!IsValid(Actor)) continue;
		AStep* Step = Cast<AStep>(Actor);
		if (!IsValid(Step)) continue;
		Steps.Add(Step);
	}
}

void UStory::DeInit_Implementation() {
	Current = nullptr;
	Steps.Empty();
}

void UStory::Start_Implementation(const FName& Name) {
	AStep *Step = nullptr;
	const int32 Num = Steps.Num();
	for (int32 i = 0; i< Num; ++i) {
		AStep* const iStep = Steps[i];
		if (iStep && IsValid(iStep) && iStep->Name == Name) {
			Step = iStep;
			break;
		}
	}
	
	if (!IsValid(Step)) {
		UE_LOG(LogTemp, Warning, TEXT("Step could not be found. '%s'"), *Name.ToString());
		return;
	}

	// Is this a good idea?
	Stop();

	UE_LOG(LogTemp, Warning, TEXT("About to start step : '%s'"), *Name.ToString());
	Current = Step;
	Step->Start();
	OnStepStart.Broadcast(Step->Name);
}

void UStory::Stop_Implementation(const FName& IfName) {
	if (!IsValid(Current)) return;
	if (!IfName.IsNone() && Current->Name != IfName) {
		UE_LOG(LogTemp, Warning, TEXT("Attempted to stop a step that is not current!!! Current='%s' ToStop='%s'"),
			*Current->Name.ToString(), *IfName.ToString());
		return;
	}

	AStep* const Step = Current;
	UE_LOG(LogTemp, Log, TEXT("About to stop step : '%s'"), *Step->Name.ToString());
	// done before calling stop to allow for other functions to call this.
	Current = nullptr;
	Step->Stop();
	OnStepStop.Broadcast(Step->Name);
	
	StartNextStep();
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

	Start(Sequence[SeqStep]);
	return true;
}

bool UStory::StartSequence_Implementation(const TArray<FName>& InSeq) {
	Sequence = InSeq;
	if (Sequence.IsEmpty()) return false;

	SeqStep = -1;
	return StartNextStep();
}
#pragma optimize("", on)
