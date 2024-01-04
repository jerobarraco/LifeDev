// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#include "Flashback.h"

#include "Interact/Animator/CAnimator.h"

DEFINE_LOG_CATEGORY_STATIC(LogFlashback, Log, Log);

UFlashback::UFlashback():Super() {
	Animator = CreateDefaultSubobject<UCAnimator>(TEXT("Animator"));
	Animator->SetComponentTickInterval(1/60.f);
	// will this package, yes it does
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
	// doesn't check the range since : it's internal, and can happen while we're animating.
	// and we want to allow a smooth transition back to a lesser min
	// don't bother if it's the same, specially since many things could be bound to onChange
	if (FMath::IsNearlyEqual(New, Val)) return;
	
	if (Debug) {
		UE_LOG(LogFlashback, Log, TEXT("Flashback:%hs Val = %.5f"), __func__, Val);
	}

	Val = New;
	OnChange.Broadcast(Val);
}

void UFlashback::AnimUpdate(float Progress, float Alpha) {
	SetValInternal(FMath::Lerp<float, float>(AnimFrom, AnimTo, Alpha));
}

void UFlashback::SetVal(float New, float Duration) {
	New = FMath::Clamp(New, Min, Max);
	const float Diff = FMath::Abs(Val - New);
	UE_LOG(LogFlashback, Log, TEXT("Flashback NewVal %.5f Diff %.5f"), New, Diff);
	if (FMath::IsNearlyZero(Diff)) return;

	// important to set, set here to keep it always up to date.
	AnimFrom = Val;
	// set instant if speed is 0
	if (FMath::IsNearlyZero(Duration)) {
		// reset animation if any
		Animator->Deactivate();
		AnimTo = New; // useless, but important to set so that the value is always up to date.
		SetValInternal(New);
		return;
	}

	// or use default anim speed
	if (Duration < 0) {
		Duration = AnimSpeed;
	}

	// important, set the actual targets.
	AnimTo = New;

	// set and play the animator
	const float Time = Duration*Diff;
	Animator->Duration = Time;
	Animator->Activate(true);
	
	UE_LOG(LogFlashback, Log, TEXT("Flashback Val %.5f Duration %.5f Time %.5f"), Val, Duration, Time);
}

void UFlashback::SetMax(float NewMax, float Duration) {
	Max = NewMax;
	
	// clamp the value if needed
	const bool Ok = Animator->IsActive() ? NewMax >= AnimTo : NewMax >= Val;
	if (Ok) return;
	SetVal(NewMax, Duration);
}

void UFlashback::SetMin(float NewMin, float Duration) {
	Min = NewMin;
	
	// clamp the value if needed
	const bool Ok = Animator->IsActive() ? NewMin <= AnimTo : NewMin <= Val;
	if (Ok) return;
	SetVal(NewMin, Duration);
}

void UFlashback::SetValS(UWorld* W, float New, float Duration) {
	UFlashback* const Flashback = Get(W);
	if (!Flashback) return;
	Flashback->SetVal(New, Duration);
}

void UFlashback::Deinitialize() {
	Animator->OnUpdate.RemoveAll(this);
	Super::Deinitialize();
}

void UFlashback::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	Animator->OnUpdate.AddUniqueDynamic(this, &UFlashback::AnimUpdate);

	// this won't make the animator work,
	// AND will make the uflashback get an EXTRA tick on a different interval (maybe the component's interval)
	// Animator->RegisterComponentWithWorld(GetWorld());
	// Animator->RegisterAllComponentTickFunctions(true);
	// Animator->RegisterComponent();
}

void UFlashback::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	// GEngine->AddOnScreenDebugMessage(33, .10f, FColor::Green,
		// TEXT("Tick %f!"));	

	// have to manually do it, not optimizations here, they are inside tickmanual
	Animator->TickManual(DeltaTime);
}

// without this it will crash. yes. it will crash. https://forums.unrealengine.com/t/how-can-i-tick-a-tickableworldsubsystem/489697/3
TStatId UFlashback::GetStatId() const {
	// https://benui.ca/unreal/tickable-object/
	// another way RETURN_QUICK_DECLARE_CYCLE_STAT( FMyTickableThing, STATGROUP_Tickables );
	return GetStatID();
}
