// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
#include "Flashback.h"

#include "Interact/Animator/CAnimator.h"

DEFINE_LOG_CATEGORY_STATIC(LogFlashback, Log, Log);

UFlashback::UFlashback():Super() {
	Animator = CreateDefaultSubobject<UCAnimator>(TEXT("Animator"));
	Animator->SetComponentTickInterval(1/60.f);
	// will this package, yes it does
	// this curve is applied to each section from->to of values. Take a look at AnimUpdate.
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		// CCurve(TEXT("/Niagara/DefaultAssets/Curves/Templates/EaseIn.EaseIn"));
		CCurve(TEXT("/Niagara/DefaultAssets/Curves/Templates/SmoothRampUp.SmoothRampUp"));
	Animator->Curve = CCurve.Succeeded() ? CCurve.Object : nullptr;
}

UFlashback* UFlashback::Instance(UObject* O) {
	if (!IsValid(O)) return nullptr;

	const UWorld* const W = O->GetWorld();
	if (!W) return nullptr;

	UFlashback* const Flashback = W->GetSubsystem<UFlashback>();
	return IsValid(Flashback) ? Flashback : nullptr;
}

void UFlashback::SetValInternal(const float New) {
	// doesn't check the range since : it's internal, and can happen while we're animating.
	// and we want to allow a smooth transition back to a lesser min
	// don't bother if it's the same, specially since many things could be bound to onChange
	if (FMath::IsNearlyEqual(New, Val)) return;
	
	if (Debug)
		UE_LOG(LogFlashback, Log, TEXT("%hs Val = %.5f"), __func__, Val);

	Val = New;
	OnChange.Broadcast(Val);
}

void UFlashback::AnimUpdate(const float Progress, const float Alpha) {
	// using From and To, keeps the animation stable and linear.
	SetValInternal(FMath::LerpStable(ValFrom, ValTo, Alpha));
}

void UFlashback::AnimEnd() {
	OnEnd.Broadcast(Val);
}

void UFlashback::SetValToInternal(const float New) {
	ValTo = New;
	OnTo.Broadcast(ValTo);
}

void UFlashback::SetVal(float New, float Duration) {
	New = FMath::Clamp(New, Min, Max);
	// critical to use ValTo and not Val here or the dialogs FBDiagMod fails on quick change.
	// keep an eye on it in case it breaks other things.
	const float Diff = FMath::Abs(ValTo - New);
	UE_LOG(LogFlashback, Log, TEXT("%hs:Pre-start Min=%.5f Max=%.5f ValTo=%.5f NewVal=%.5f Diff=%.5f "),
		__func__, Min, Max, ValTo, New, Diff);
	if (FMath::IsNearlyZero(Diff)) return;

	// important to set, set here to keep it always up to date.
	ValFrom = Val;
	// set instant if speed is 0
	if (FMath::IsNearlyZero(Duration)) {
		// reset animation if any
		Animator->Deactivate();
		SetValToInternal(New);
		// important to set so that the value is always up-to-date.
		// since it's used for GetValTo and in turn by SetVal
		SetValInternal(New);
		AnimEnd(); // force notify even though it's instant.
		return;
	}

	// or use default anim speed
	if (Duration < 0) Duration = AnimSpeed;

	// important, set the actual targets.
	ValTo = New;

	// set and play the animator
	const float Time = Duration*Diff;
	Animator->Duration = Time;
	Animator->Activate(true);
	
	UE_LOG(LogFlashback, Log, TEXT("%hs:Start Val=%.5f ValTo=%.5f Duration=%.5f Time=%.5f"),
		__func__, Val, ValTo, Duration, Time);

	OnTo.Broadcast(ValTo);
}

void UFlashback::SetMax(const float NewMax, const float Duration) {
	UE_LOG(LogFlashback, Log, TEXT("%hs Old=%.5f New=%.5f Duration=%.5f"),
		__func__, Max, NewMax, Duration);

	Max = NewMax;
	
	// clamp the value if needed
	const bool Ok = NewMax >= GetValTo();
	if (Ok) return;
	SetVal(NewMax, Duration);
}

void UFlashback::SetMin(const float NewMin, const float Duration) {
		UE_LOG(LogFlashback, Log, TEXT("%hs Old=%.5f New=%.5f Duration=%.5f"),
		__func__, Min, NewMin, Duration);
	Min = NewMin;
	
	// clamp the value if needed
	const bool Ok = NewMin <= GetValTo();
	if (Ok) return;

	SetVal(NewMin, Duration);
}

void UFlashback::Deinitialize() {
	if (Animator) {
		Animator->OnUpdate.RemoveAll(this);
		Animator->OnEnd.RemoveAll(this);
	}
	Super::Deinitialize();
}

void UFlashback::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	Animator->OnUpdate.AddUniqueDynamic(this, &UFlashback::AnimUpdate);
	Animator->OnEnd.AddUniqueDynamic(this, &UFlashback::AnimEnd);

	// this won't make the animator work,
	// AND will make the uflashback get an EXTRA tick on a different interval (maybe the component's interval)
	// Animator->RegisterComponentWithWorld(GetWorld());
	// Animator->RegisterAllComponentTickFunctions(true);
	// Animator->RegisterComponent();
}

void UFlashback::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	#if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(33, DeltaTime, FColor::Green,
			TEXT("Flashback Tick."), true);
	#endif

	// have to manually do it, not optimizations here, they are inside tickmanual
	Animator->TickManual(DeltaTime);
}

// without this it will crash. yes. it will crash. https://forums.unrealengine.com/t/how-can-i-tick-a-tickableworldsubsystem/489697/3
TStatId UFlashback::GetStatId() const {
	// https://benui.ca/unreal/tickable-object/
	// another way RETURN_QUICK_DECLARE_CYCLE_STAT( FMyTickableThing, STATGROUP_Tickables );
	return GetStatID();
}

bool UFlashback::IsTickable() const {
	return Animator && Animator->IsActive(); // small optimization
}
