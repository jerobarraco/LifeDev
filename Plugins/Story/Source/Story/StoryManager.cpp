// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "StoryManager.h"

#include "Step.h"

void AStoryManager::Init_Implementation() {}
void AStoryManager::DeInit_Implementation() {}

void AStoryManager::Start_Implementation(const FName& Name) {
	AStep** pStep = Steps.Find(Name);
	if (!pStep || !IsValid(*pStep)) {
		UE_LOG(LogTemp, Warning, TEXT("Step could not be found. '%s'"), *Name.ToString());
		return;
	}
	AStep* Step = *pStep;
	Step->Start();
}
