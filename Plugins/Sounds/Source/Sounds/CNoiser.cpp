// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CNoiser.h"

#include "Kismet/GameplayStatics.h"

UCNoiser::UCNoiser():Super() {
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
	Super::SetAutoActivate(false);
}

void UCNoiser::Activate(bool bReset) {
	Super::Activate(bReset);
	UE_LOG(LogTemp, Log, TEXT("UCNoiser::%hs"), __func__);
	if (IsActive() && !bReset) return;
	
	TimerStart(); // already checks for IsPlaying
}

void UCNoiser::Deactivate() {
	UE_LOG(LogTemp, Log, TEXT("UCNoiser::hs %s"), __func__);
	TimerStop();
	Super::Deactivate();
}

void UCNoiser::TimerStop() {
	UE_LOG(LogTemp, Log, TEXT("UCNoiser::hs %s"), __func__);
	const UWorld* const World = GetWorld();
	if (!World || !TimerPlay.IsValid()) return;

	World->GetTimerManager().ClearTimer(TimerPlay);
	TimerPlay.Invalidate();
}

void UCNoiser::TimerStart() {
	UE_LOG(LogTemp, Log, TEXT("UCNoiser::hs %s"), __func__);

	const UWorld* const World = GetWorld();
	if (!World || !IsPlaying) return;
	const float Time = FMath::FRandRange(TimeMin, TimeMax);
	
	World->GetTimerManager().SetTimer(TimerPlay, this, &UCNoiser::PlayNow, Time, false, -1);
}

void UCNoiser::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Deactivate();
	Super::EndPlay(EndPlayReason);
}

void UCNoiser::PlayNow_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("UCNoiser::hs %s"), __func__);
	const UWorld* const World = GetWorld();
	if (!World) {
		return;
	}

	const AActor* const Owner = GetOwner();
	if (!IsValid(Owner)) {
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
	UE_LOG(LogTemp, Log, TEXT("UCNoiser::hs at %s"), __func__, *Location.ToString());

	// force the soundclass, used by child classes.
	if (SoundClass) SFX->SoundClassObject = SoundClass;

	UGameplayStatics::PlaySoundAtLocation(
		World, SFX, Location, Rotation, 1, 1, 0,
		Attenuation
	);
	
	TimerStart(); // starts the next one.

	OnPlay.Broadcast();
}

