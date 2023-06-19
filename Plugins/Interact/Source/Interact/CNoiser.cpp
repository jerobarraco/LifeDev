// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

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
	const FVector OwnerBwd = -OwnerFwd;
	const FVector& Dir = FMath::VRandCone(OwnerBwd, FMath::DegreesToRadians(HalfRadius));
	
	FVector Location = OwnerLocation + (Dir*DistMax);
	const FRotator& Rotation = (OwnerLocation - Location).Rotation();
	if (Debug) {
		DrawDebugLine(GetWorld(), Location, Location + (Rotation.Vector() * DistMax), FColor::Purple, false, 4);
	}
	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(), SFX, Location, Rotation,
		1, 1, 0,
		Attenuation
	);
	
	TimerStart();

	OnPlay.Broadcast();
}

