// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "StoryManager.h"

#include "Step.h"

void AStoryManager::Init_Implementation() {}
void AStoryManager::DeInit_Implementation() {}

void AStoryManager::Start_Implementation(const FName& Name) {
	AStep *Step = nullptr;
	for (int32 i = 0; Steps.Num(); ++i) {
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
	Step->Start();
}
