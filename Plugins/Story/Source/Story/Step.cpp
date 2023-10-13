// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#include "Step.h"

#include "Kismet/GameplayStatics.h"

#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

#include "Story.h"

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

	// teleport the character
	// teleport before blending the camera. so they work well together.
	if (World && TeleportChar) {
        ACharacter* const Char = Cast<ACharacter>(
        	UGameplayStatics::GetActorOfClass(World, ACharacter::StaticClass()));
        if (IsValid(Char)) {
        	const FTransform& T = GetActorTransform();
        	Char->TeleportTo(T.GetLocation(), T.Rotator());
        }
	}

	// blend before the wait to avoid weird issues.
	// if you actually wanna see the blend you may not want the fade anyway. fade and wait are weird combination. i think.
	BlendCam();

	// do after the rest since post-wait is another flow
	if (World && WaitTime>0) {
		FTimerHandle Handle;
		World->GetTimerManager().SetTimer(Handle, this, &AStep::PostWait, WaitTime);
	} else {
		PostWait();
	}
}

void AStep::BlendCam() const {
	// set camera if camtarget is set
	if (!IsValid(CamTarget)) return;

	UE_LOG(LogTemp, Log, TEXT("AStep Blending camera"));
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(CamTarget, CamBlendTime, VTBlend_Cubic);
}

void AStep::PostWait_Implementation() {
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

void AStep::UpdateCamEnabled() const {
	const bool Enabled = CamTarget == this;
	if (!IsValid(Cam)) return;
	
	Cam->SetActive(Enabled);
	Cam->SetHiddenInGame(!Enabled);
	Cam->SetVisibility(Enabled);
	Cam->SetComponentTickEnabled(Enabled);
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
