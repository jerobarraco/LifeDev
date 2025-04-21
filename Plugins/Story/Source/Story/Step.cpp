// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

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

#if WITH_EDITORONLY_DATA
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CCam (TEXT("/Niagara/DefaultAssets/S_Arrow.S_Arrow"));
	if (CCam.Succeeded()) Cam->SetCameraMesh(CCam.Object);
#endif

	CamTarget = this;
}

void AStep::BeginPlay() {
	Super::BeginPlay();

	if (UNLIKELY(Name.IsNone())) {
		UE_LOG(LogStoryStep, Warning, TEXT("%hs Step name is none!"
			" Step won't work properly, so not adding to the story. Stop."), __func__);
		return;
	}

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	UStory* const Story = World->GetSubsystem<UStory>();
	if (UNLIKELY(!Story)) return;
 
	Story->Add(this);

	if (UsePawnCam) {
		AActor* const Actor = UGameplayStatics::GetActorOfClass(World, APawn::StaticClass());
		APawn* const Pawn = Cast<APawn>(Actor);
		if (LIKELY(IsValid(Pawn))) {
			CamTarget = Pawn;
		} else {
			UE_LOG(LogStoryStep, Warning, TEXT("%hs UsePawnCam set but could not get cam. Won't work as expected."),
			__func__);
		}
	}
}

void AStep::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	// important for weird states. so that it doesn't trigger other actions when the world is destroyed just when it's starting.
	World->GetTimerManager().ClearAllTimersForObject(this);

	UStory* const Story = World->GetSubsystem<UStory>();
	if (LIKELY(IsValid(Story))) Story->Rem(Name);

	Super::EndPlay(EndPlayReason);
}

void AStep::PostLoad() {
	Super::PostLoad();
	UpdateCamEnabled();
}

void AStep::TryStart_Implementation() {
	UE_LOG(LogStoryStep, Log, TEXT("%hs Starting step '%s'"), __func__, *Name.ToString());
	
	// teleport the character
	// teleport before blending the camera. so they work well together.
	DoTeleport();

	// blend before the wait to avoid weird issues.
	// if you actually want to see the blend you may not want the fade anyway.
	// fade and wait are weird combination. i think.
	CamBlend();

	// do after the rest since doStart is another flow
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	
	if (WaitTime>0) {
		FTimerHandle Handle;
		World->GetTimerManager().SetTimer(Handle, this, &AStep::Start, WaitTime);
	} else {
		// use next tick to avoid having post wait being called before start finishes on the children
		// also to avoid the situation where a step might inadvertently finish the step while it's starting.
		World->GetTimerManager().SetTimerForNextTick(this, &AStep::Start);
	}
}

void AStep::Start_Implementation() {
	UE_LOG(LogStoryStep, Log, TEXT("%hs -> %s"), __func__, *Name.ToString());

	if (UNLIKELY(Debug)) DoDebug();
	// check UseCamShake outside CamShakeStart to allow children to call it.
	if (UseCamShake) CamShakeStart();

	/// finish post wait
	// do on next tick to avoid issues on classes inheriting this or subscribed to delegates.
	if (!FinishPostWait) return;
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	World->GetTimerManager().SetTimerForNextTick(this, &AStep::Finish);
}

void AStep::Stop_Implementation() {
	UE_LOG(LogStoryStep, Log, TEXT("%hs Stopping step '%s'"), __func__, *Name.ToString());
	// force disable since it's not wise to trust what happened before
	if (UNLIKELY(IsValid(Cam))) Cam->SetComponentTickEnabled(false);
	if (UseCamShake) CamShakeStop();
}

void AStep::Finish_Implementation() {
	UE_LOG(LogStoryStep, Log, TEXT("%hs Finishing step '%s'"), __func__, *Name.ToString());

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;

	// important for weird states. so that it doesn't trigger other actions when the world is destroyed just when it's starting.
	World->GetTimerManager().ClearAllTimersForObject(this);

	UStory* const Story = World->GetSubsystem<UStory>();
	if (UNLIKELY(!IsValid(Story))) return;

	Story->StartNext(Name);
}

void AStep::DoTeleport() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	if (UNLIKELY(!TeleportChar)) return;

	ACharacter* const Char = Cast<ACharacter>(
		UGameplayStatics::GetActorOfClass(World, ACharacter::StaticClass()));
	if (UNLIKELY(!IsValid(Char))) return;

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
	if (UNLIKELY(!IsValid(C))) return;

	Char->AddControllerPitchInput(T.Rotator().Pitch - C->GetRelativeRotation().Pitch);
}

void AStep::CamBlend() {
	// set camera if camtarget is set
	if (!IsValid(CamTarget)) return;
	UE_LOG(LogStoryStep, Log, TEXT("%hs -> %s"), __func__, *Name.ToString());

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	APlayerController* const Controller = World->GetFirstPlayerController();
	if (UNLIKELY(!Controller)) return;

	// enable cam tick only if it's the current target and only when the step starts
	if (CamTarget == this && LIKELY(IsValid(Cam))) Cam->SetComponentTickEnabled(true);

	Controller->SetViewTargetWithBlend(CamTarget, CamBlendTime, VTBlend_Cubic);
	// ensure the step does not start before the camera ends the blending
	WaitTime = FMath::Max(CamBlendTime, WaitTime);
}

void AStep::UpdateCamEnabled() const {
	const bool Enabled = CamTarget == this;
	if (UNLIKELY(!IsValid(Cam))) return;
	
	Cam->SetActive(Enabled);
	Cam->SetHiddenInGame(!Enabled);
	Cam->SetVisibility(Enabled);
	// the tick is enabled on start. only if needed
}

void AStep::CamShakeStart() {
	if (!IsValid(CamShakeClass)) return;

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	const APlayerController* const Controller = World->GetFirstPlayerController();
	if (UNLIKELY(!Controller)) return;

	const TObjectPtr<APlayerCameraManager> CameraManager =  Controller->PlayerCameraManager;
	// immediate needed since the shake has no end (gimme shake - Max.avi)
	if (LIKELY(CameraManager)) CameraManager->StartCameraShake(CamShakeClass);
}

void AStep::CamShakeStop() {
	if (!IsValid(CamShakeClass)) return;

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	const APlayerController* const Controller = World->GetFirstPlayerController();
	if (UNLIKELY(!Controller)) return;

	const TObjectPtr<APlayerCameraManager> CameraManager =  Controller->PlayerCameraManager;
	// immediate needed since the shake has no end (gimme shake - Max.avi)
	if (LIKELY(CameraManager))
		CameraManager->StopAllInstancesOfCameraShake(CamShakeClass, true);
}
