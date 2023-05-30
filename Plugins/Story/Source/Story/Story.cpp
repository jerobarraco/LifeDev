// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "Story.h"

#include "Step.h"

#include "Kismet/GameplayStatics.h"

#pragma optimize("", off)

#pragma optimize("", on)
void UStory::Init_Implementation() {
	TArray<AActor*> Actors;
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
}

void UStory::Stop_Implementation(const FName& WithName) {
	if (!IsValid(Current)) return;
	if (!WithName.IsNone() && Current->Name != WithName) {
		UE_LOG(LogTemp, Warning, TEXT("Attempted to stop a step that is not current!!! Current='%s' ToStop='%s'"),
			*Current->Name.ToString(), *WithName.ToString());
		return;
	}

	AStep* Step = Current;
	UE_LOG(LogTemp, Log, TEXT("About to stop step : '%s'"), *Step->Name.ToString());
	Current = nullptr; // done before calling stop to allow for other functions to call this.
	Step->Stop();
}
