// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#include "Step.h"

#include "Kismet/GameplayStatics.h"

#include "Story.h"
#include "Camera/CameraComponent.h"

AStep::AStep():Super() {
	PrimaryActorTick.bCanEverTick = false;
	Super::SetActorTickEnabled(false);
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	Cam->SetupAttachment(Root);
	// Cam->SetComponentTickEnabled(false);
	CamTarget = this;
}

void AStep::Start_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Starting step '%s'"), *Name.ToString());

	UWorld* const World = GetWorld();
	if (WaitTime>0) {
		FTimerHandle Handle;
		World->GetTimerManager().SetTimer(Handle, this, &AStep::PostWait, WaitTime);
	} else {
		PostWait();
	}
}

void AStep::PostWait_Implementation() {
	// set camera if camtarget is set
	if (IsValid(CamTarget)) {
		UE_LOG(LogTemp, Log, TEXT("AStep Blending camera"));
		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(CamTarget, CamBlendTime, VTBlend_Cubic);
	}
	
	if (UseDebug) {
		Debug();
	}
	
	if (FinishPostWait) {
		Finish();
	}
}

void AStep::BeginPlay() {
	Super::BeginPlay();
	if (Name.IsNone()) {
		UE_LOG(LogTemp, Warning, TEXT("Step name is none! story won't work properly"));
	}

	UWorld* const World = GetWorld();
	UStory* const Story = World->GetSubsystem<UStory>();
	Story->Add(this);
	
	if (UsePawnCam) {
		AActor* const Actor = UGameplayStatics::GetActorOfClass(World, APawn::StaticClass());
		APawn* const Pawn = Cast<APawn>(Actor);
		if (!IsValid(Pawn)) {
			UE_LOG(LogTemp, Warning, TEXT("Could not get the pawn!!!!"));
		} else {
			CamTarget = Pawn;
		}
	}
}

void AStep::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UStory* const Story = GetWorld()->GetSubsystem<UStory>();
	Story->Rem(Name);

	Super::EndPlay(EndPlayReason);
}

void AStep::PostLoad() {
	Super::PostLoad();
	UpdateCamEnabled();
}

void AStep::UpdateCamEnabled() {
	const bool Enabled = CamTarget == this;
	if (IsValid(Cam)) {
		Cam->SetActive(Enabled);
		Cam->SetHiddenInGame(!Enabled);
		Cam->SetVisibility(Enabled);
		Cam->SetComponentTickEnabled(Enabled);
	}
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
	Story->Stop(Name);
}
