// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "CNoiser.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "Sound/SoundBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogCNoiser, Log, Log);

UCNoiser::UCNoiser():Super() {
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
	Super::SetAutoActivate(false);
}

void UCNoiser::Activate(const bool bReset) {
	UE_LOG(LogCNoiser, Log, TEXT("%hs Reset=%i"), __func__, bReset);
	if (IsActive() && !bReset) return;

	// has to be after the IsActive check!
	Super::Activate(bReset);
	
	TimerStart(); // already checks for IsPlaying
}

void UCNoiser::Deactivate() {
	UE_LOG(LogCNoiser, Log, TEXT("%hs"), __func__);
	TimerStop();
	Super::Deactivate();
}

void UCNoiser::TimerStop() {
	UE_LOG(LogCNoiser, Log, TEXT("%hs"), __func__);
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World) || !TimerPlay.IsValid()) return;

	World->GetTimerManager().ClearTimer(TimerPlay);
	TimerPlay.Invalidate();
}

void UCNoiser::TimerStart() {
	UE_LOG(LogCNoiser, Log, TEXT("%hs"), __func__);

	const UWorld* const World = GetWorld();
	if (!World) return;

	if (IsPlaying) {
		UE_LOG(LogCNoiser, Log, TEXT("%hs IsPlaying true. skip."), __func__);
		return;
	}

	const float Time = FMath::FRandRange(TimeMin, TimeMax);
	World->GetTimerManager().SetTimer(TimerPlay, this, &UCNoiser::PlayNow, Time, false, -1);
}

void UCNoiser::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Deactivate();
	Super::EndPlay(EndPlayReason);
}

void UCNoiser::PlayNow_Implementation() {
	UE_LOG(LogCNoiser, Log, TEXT("%hs"), __func__);
	const UWorld* const World = GetWorld();
	if (!World) return;

	const AActor* const Owner = GetOwner();
	if (!IsValid(Owner)) {
		UE_LOG(LogCNoiser, Log, TEXT("%hs No owner. Stopping."), __func__);
		Deactivate();
		return;
	}

	const FVector& OwnerLocation = Owner->GetActorLocation();
	const FVector& OwnerFwd = Owner->GetActorForwardVector();
	// actually point at the back
	const FVector& OwnerBwd = -OwnerFwd;

	const float AngleWidth = FMath::DegreesToRadians(HalfAngleWidth);
	const float AngleHeight = FMath::DegreesToRadians(HalfAngleHeight);
	const FVector& Dir = FMath::VRandCone(
		OwnerBwd,AngleWidth, AngleHeight);
	const float Dist = FMath::FRandRange(DistMin, DistMax);
	const FVector& Location = OwnerLocation + (Dir*Dist);
	const FRotator& Rotation = (OwnerLocation - Location).Rotation();
	
	if (Debug) {
		DrawDebugLine(World, Location, Location + (Rotation.Vector() * Dist), FColor::Purple, false, 4);
		DrawDebugPoint(World, Location, 4, FColor::Green, false, 4);
		DrawDebugPoint(World, OwnerLocation, 4, FColor::Red, false, 4);
		DrawDebugCone(World, OwnerLocation, OwnerBwd, Dist,  AngleWidth, AngleHeight, 20, FColor::Silver, false, 4);
	}
	
	UE_LOG(LogCNoiser, Log, TEXT("%hs Play at %s"), __func__, *Location.ToString());
	// force the soundclass, used by child classes.
	if (SoundClass) SFX->SoundClassObject = SoundClass;

	UGameplayStatics::PlaySoundAtLocation(
		World, SFX, Location, Rotation, 1, 1, 0,
		Attenuation
	);
	
	TimerStart(); // starts the next one.
	OnPlay.Broadcast();
}

