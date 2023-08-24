// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CNoiser.h"

#include "Kismet/GameplayStatics.h"

UCNoiser::UCNoiser():Super() {
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.SetTickFunctionEnable(false);
}

void UCNoiser::TimerStop() {
	if (Handle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(Handle);
		Handle.Invalidate();
	}
}

void UCNoiser::TimerStart() {
	if (!IsPlaying) return;
	const float Time = FMath::FRandRange(TimeMin, TimeMax);
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &UCNoiser::PlayNow, Time, false, -1);
}

void UCNoiser::Start() {
	SetIsPlaying(true);
}

void UCNoiser::Stop() {
	SetIsPlaying(false);
}

void UCNoiser::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Stop();
	Super::EndPlay(EndPlayReason);
}

void UCNoiser::SetIsPlaying(bool NewIsPlaying) {
	const bool WasPlaying = IsPlaying;
	IsPlaying = NewIsPlaying;

	TimerStop();
	if (IsPlaying) {
		TimerStart();
	}
}

void UCNoiser::PlayNow_Implementation() {
	AActor* const Owner = GetOwner();
	if (!IsValid(Owner)) {
		Stop();
		return;
	}

	const FVector& OwnerLocation = Owner->GetActorLocation();
	const FVector& OwnerFwd = Owner->GetActorForwardVector();
	// actually point at the back
	const FVector OwnerBwd = -OwnerFwd;

	const float AngleWidth = FMath::DegreesToRadians(HalfAngleWidth);
	const float AngleHeight = FMath::DegreesToRadians(HalfAngleHeight);
	const FVector& Dir = FMath::VRandCone(
		OwnerBwd,AngleWidth, AngleHeight);
	const float Dist = FMath::FRandRange(DistMin, DistMax);
	FVector Location = OwnerLocation + (Dir*Dist);
	const FRotator& Rotation = (OwnerLocation - Location).Rotation();
	
	UWorld* const World = GetWorld();
	if (Debug) {
		DrawDebugLine(World, Location, Location + (Rotation.Vector() * Dist), FColor::Purple, false, 4);
		DrawDebugPoint(World, Location, 4, FColor::Green, false, 4);
		DrawDebugPoint(World, OwnerLocation, 4, FColor::Red, false, 4);
		DrawDebugCone(World, OwnerLocation, OwnerBwd, Dist,  AngleWidth, AngleHeight, 20, FColor::Silver, false, 4);
	}
	UE_LOG(LogTemp, Log, TEXT("Noiser at %s"), *Location.ToString());
	
	UGameplayStatics::PlaySoundAtLocation(
		World, SFX, Location, Rotation,
		1, 1, 0,
		Attenuation
	);
	
	TimerStart();

	OnPlay.Broadcast();
}

