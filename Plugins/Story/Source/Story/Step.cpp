// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#include "Step.h"

#include "Kismet/GameplayStatics.h"

#include "Story.h"

AStep::AStep():Super() {
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
}

void AStep::Start_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Starting step '%s'"), *Name.ToString());

	if (IsPawnTarget) {
		AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), APawn::StaticClass());
		APawn* Pawn = Cast<APawn>(Actor);
		if (!IsValid(Pawn)) {
			UE_LOG(LogTemp, Warning, TEXT("Could not get the pawn!!!!"));
		} else {
			CamTarget = Pawn;
		}
	}
	
	if (IsValid(CamTarget)) {
		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(CamTarget, 1, VTBlend_EaseInOut);
	}
}

void AStep::BeginPlay() {
	Super::BeginPlay();
	UStory* const Story = GetWorld()->GetSubsystem<UStory>();
	Story->Add(this);
}

void AStep::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UStory* const Story = GetWorld()->GetSubsystem<UStory>();
	Story->Rem(this->Name);

	Super::EndPlay(EndPlayReason);
}

void AStep::Stop_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Stopping step '%s'"), *Name.ToString());
}

void AStep::Finish_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Finishing step '%s'"), *Name.ToString());

	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	UStory* const Story = World->GetSubsystem<UStory>();
	if (!IsValid(Story)) return;
	// TODO should i make this a delegate instead?

	Story->Stop(Name);
}
