// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "Step.h"

#include "Story.h"
#include "Camera/CameraActor.h"

AStep::AStep():Super() {
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
}

void AStep::Start_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Starting step '%s'"), *Name.ToString());

	if (IsValid(Camera)) {
		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Camera, 1, VTBlend_EaseInOut);
	}
}

void AStep::Stop_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Stopping step '%s'"), *Name.ToString());
}

// TODO should i invert this with stop???
void AStep::Finish_Implementation() {
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	UStory* const Story = World->GetSubsystem<UStory>();
	if (!IsValid(Story)) return;
	// TODO should i make this a delegate instead?

	Story->Stop(Name);
}
