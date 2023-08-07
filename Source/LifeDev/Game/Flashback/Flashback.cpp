// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#include "Flashback.h"

#include "Interact/Animator/CAnimator.h"

UFlashback::UFlashback():Super() {
	Animator = CreateDefaultSubobject<UCAnimator>(TEXT("Animator"));
}

void UFlashback::SetValInternal(float New) {
	New = FMath::Clamp(New, 0.0f, 1.0f);
	if (FMath::IsNearlyEqual(New, Val)) return;
	Val = New;
	UE_LOG(LogTemp, Log, TEXT("Updating value to %.5f"), Val);
	OnChange.Broadcast(Val);
}

void UFlashback::AnimUpdate(float Progress, float Alpha) {
	SetValInternal(FMath::Lerp(AnimFrom, AnimTo, Alpha));
}

void UFlashback::IncVal(float By, float Speed) {
	SetVal(Val+By, Speed);
}

void UFlashback::SetVal(float New, float Speed) {
	const float Diff = FMath::Abs(Val - New);
	// UE_LOG(LogTemp, Log, TEXT("Diff %.5f"),  Diff);
	if (FMath::IsNearlyZero(Diff)) return;

	// reset animation if any
	Animator->Stop();

	// set instant if speed is 0
	if (FMath::IsNearlyZero(Speed)) {
		SetValInternal(New);
		return;
	}

	// or use default anim speed
	if (Speed < 0) {
		Speed = AnimSpeed;
	}

	// important, set the actual targets.
	AnimFrom = Val;
	AnimTo = New;

	// set and play the animator
	const float Time = Speed*Diff;
	Animator->Duration = Time;
	Animator->Play();
	// UE_LOG(LogTemp, Log, TEXT("Speed, Time %.5f %.5f"), Speed, Time);
}

void UFlashback::Deinitialize() {
	Super::Deinitialize();
	Animator->OnUpdate.RemoveAll(this);
}

void UFlashback::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	Animator->OnUpdate.AddUniqueDynamic(this, &UFlashback::AnimUpdate);

	// doesn't do anything. but maybe it helps, or maybe it crashes.
	Animator->RegisterComponentWithWorld(GetWorld());
	Animator->RegisterAllComponentTickFunctions(true);
	// Animator->RegisterComponent();
}

void UFlashback::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	// GEngine->AddOnScreenDebugMessage(33, .10f, FColor::Green,
		// TEXT("Tick %f!"));	

	// have to manually do it. no optimizations here.
	Animator->DoTick(DeltaTime);
}

// without this it will crash. yes. it will crash. https://forums.unrealengine.com/t/how-can-i-tick-a-tickableworldsubsystem/489697/3
TStatId UFlashback::GetStatId() const {
	return GetStatID();
}
