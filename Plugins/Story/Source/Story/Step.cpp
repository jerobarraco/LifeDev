// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

#include "Step.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

#include "Story.h"

DEFINE_LOG_CATEGORY_STATIC(LogStoryStep, Log, Log);

AStep::AStep():Super() {
	PrimaryActorTick.bCanEverTick = false;
	Super::SetActorTickEnabled(false);
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	Cam->SetupAttachment(Root);
	Cam->SetComponentTickEnabled(false);
	Cam->SetVisibility(false);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CCam (TEXT("/Niagara/DefaultAssets/S_Arrow.S_Arrow"));
	if (CCam.Succeeded()) Cam->SetCameraMesh(CCam.Object);
	CamTarget = this;
}

void AStep::DoTeleport() {
	UWorld* const World = GetWorld();
	if (!World) return;
	
	if (!TeleportChar) return;

	ACharacter* const Char = Cast<ACharacter>(
		UGameplayStatics::GetActorOfClass(World, ACharacter::StaticClass()));
	if (!IsValid(Char)) return;
	
	const FTransform& T = GetActorTransform();
	UE_LOG(LogStoryStep, Log, TEXT("Teleport to=%s"), *T.ToString());
	// Char->TeleportTo(T.GetLocation(), T.Rotator());
	Char->SetActorLocation(T.GetLocation());

	const FRotator CurRot = Char->GetActorRotation();
	// In order for this to work the legacy input scale must be disabled on the project settings.
	// "EnableLegacyInputScales"
	Char->AddControllerYawInput(T.Rotator().Yaw-CurRot.Yaw);
	
	// vertical is handled by the camera
	TArray<UCameraComponent*> Cams; 
	Char->GetComponents<UCameraComponent>(Cams);
	if (Cams.Num()<=0) return;

	const UCameraComponent* const C = Cams[0];
	// if (!IsValid(C) || !C->bUsePawnControlRotation) return;
	if (!IsValid(C)) return;

	Char->AddControllerPitchInput(T.Rotator().Pitch-C->GetRelativeRotation().Pitch);
}

void AStep::Start_Implementation() {
	UE_LOG(LogStoryStep, Log, TEXT("Starting step '%s'"), *Name.ToString());
	
	// teleport the character
	// teleport before blending the camera. so they work well together.
	DoTeleport();

	// enable cam tick only if it's the current target and only when the step starts
	if (CamTarget == this && IsValid(Cam)) Cam->SetComponentTickEnabled(true);

	// blend before the wait to avoid weird issues.
	// if you actually wanna see the blend you may not want the fade anyway.
	// fade and wait are weird combination. i think.
	BlendCam();

	// do after the rest since post-wait is another flow
	UWorld* const World = GetWorld();
	if (!World) return;
	
	if (WaitTime>0) {
		FTimerHandle Handle;
		World->GetTimerManager().SetTimer(Handle, this, &AStep::PostWait, WaitTime);
	} else {
		// use next tick to avoid having post wait being called before start finishes on the children
		// also to avoid the situation where a step might inadvertently finish the step while it's starting.  
		World->GetTimerManager().SetTimerForNextTick(this, &AStep::PostWait);
	}
}

void AStep::BlendCam() const {
	// set camera if camtarget is set
	if (!IsValid(CamTarget)) return;
	UE_LOG(LogStoryStep, Log, TEXT("%hs -> %s"), __func__, *Name.ToString());

	UWorld* const World = GetWorld();
	if (!World) return;
	APlayerController* const Controller = World->GetFirstPlayerController();
	if (!Controller) return;
	Controller->SetViewTargetWithBlend(CamTarget, CamBlendTime, VTBlend_Cubic);
}

void AStep::PostWait_Implementation() {
	UE_LOG(LogStoryStep, Log, TEXT("%hs -> %s"), __func__, *Name.ToString());

	if (Debug) {
		DoDebug();
	}
	
	if (FinishPostWait) {
		Finish();
	}
}

void AStep::BeginPlay() {
	Super::BeginPlay();
	if (Name.IsNone()) {
		UE_LOG(LogStoryStep, Warning, TEXT("Step name is none! Step won't work properly, so not adding to the story."));
		return;
	}

	UWorld* const World = GetWorld();
	if (!World) return;
	UStory* const Story = World->GetSubsystem<UStory>();
	if (!Story) return;
	Story->Add(this);
	
	if (UsePawnCam) {
		AActor* const Actor = UGameplayStatics::GetActorOfClass(World, APawn::StaticClass());
		APawn* const Pawn = Cast<APawn>(Actor);
		if (!IsValid(Pawn)) {
			UE_LOG(LogStoryStep, Warning, TEXT("UsePawnCam set but could not get cam. Won't work as expected."));
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
	// the tick is enabled on start. only if needed
}

void AStep::Stop_Implementation() {
	UE_LOG(LogStoryStep, Log, TEXT("Stopping step '%s'"), *Name.ToString());
	// force disable since it's not wise to trust what happened before
	if (IsValid(Cam)) Cam->SetComponentTickEnabled(false);
}

void AStep::Finish_Implementation() {
	UE_LOG(LogStoryStep, Log, TEXT("Finishing step '%s'"), *Name.ToString());

	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;

	UStory* const Story = World->GetSubsystem<UStory>();
	if (!IsValid(Story)) return;
	Story->StartNext(Name);
}
