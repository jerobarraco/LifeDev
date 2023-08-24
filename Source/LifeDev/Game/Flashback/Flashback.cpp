// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#include "Flashback.h"

#include "Interact/Animator/CAnimator.h"

UFlashback::UFlashback():Super() {
	Animator = CreateDefaultSubobject<UCAnimator>(TEXT("Animator"));
	Animator->SetComponentTickInterval(1/30.f);
	// TODO will this package?
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCurve(TEXT("/Niagara/DefaultAssets/Curves/Templates/EaseIn.EaseIn"));
	Animator->Curve = CCurve.Succeeded() ? CCurve.Object : nullptr;
}

UFlashback* UFlashback::Get(UWorld* W) {
	if (!IsValid(W)) return nullptr;
	UFlashback* const Flashback = W->GetSubsystem<UFlashback>();
	return IsValid(Flashback) ? Flashback : nullptr;
}

void UFlashback::SetValInternal(float New) {
	// make sure is on range. it can break other stuff.
	New = FMath::Clamp(New, 0.0f, 1.0f);
	// don't bother if it's the same, specially since many things could be bound to onChange
	if (FMath::IsNearlyEqual(New, Val)) return;
	
	if (Debug) {
		// UE_LOG(LogTemp, Log, TEXT("Flashback Val = %.5f"), Val);
	}

	Val = New;
	OnChange.Broadcast(Val);
}

void UFlashback::AnimUpdate(float Progress, float Alpha) {
	SetValInternal(FMath::Lerp(AnimFrom, AnimTo, Alpha));
}

void UFlashback::IncVal(float By, float Duration) {
	SetVal(Val+By, Duration);
}

void UFlashback::SetVal(float New, float Duration) {
	const float Diff = FMath::Abs(Val - New);
	UE_LOG(LogTemp, Log, TEXT("Flashback NewVal %.5 Diff %.5f"), New, Diff);
	if (FMath::IsNearlyZero(Diff)) return;

	// reset animation if any
	Animator->Stop();

	// set instant if speed is 0
	if (FMath::IsNearlyZero(Duration)) {
		SetValInternal(New);
		return;
	}

	// or use default anim speed
	if (Duration < 0) {
		Duration = AnimSpeed;
	}

	// important, set the actual targets.
	AnimFrom = Val;
	AnimTo = New;

	// set and play the animator
	const float Time = Duration*Diff;
	Animator->Duration = Time;
	Animator->PlaySet();
	UE_LOG(LogTemp, Log, TEXT("Flashback Val %.5f Speed %.5 Time %.5f"), Val, Duration, Time);
}

void UFlashback::SetValS(UWorld* W, float New, float Duration) {
	UFlashback* Flashback = Get(W);
	if (!Flashback) return;
	Flashback->SetVal(New, Duration);
}

void UFlashback::Deinitialize() {
	Super::Deinitialize();
	Animator->OnUpdate.RemoveAll(this);
}

void UFlashback::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	Animator->OnUpdate.AddUniqueDynamic(this, &UFlashback::AnimUpdate);

	// this won't make the animator work, but will make the uflashback get an EXTRA tick on a different interval (maybe the component's interval)
	// Animator->RegisterComponentWithWorld(GetWorld());
	// Animator->RegisterAllComponentTickFunctions(true);
	// Animator->RegisterComponent();
}

void UFlashback::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	// GEngine->AddOnScreenDebugMessage(33, .10f, FColor::Green,
		// TEXT("Tick %f!"));	

	// have to manually do it. no optimizations here.
	Animator->TickManual(DeltaTime);
}

// without this it will crash. yes. it will crash. https://forums.unrealengine.com/t/how-can-i-tick-a-tickableworldsubsystem/489697/3
TStatId UFlashback::GetStatId() const {
	// https://benui.ca/unreal/tickable-object/
	// another way RETURN_QUICK_DECLARE_CYCLE_STAT( FMyTickableThing, STATGROUP_Tickables );
	return GetStatID();
}
