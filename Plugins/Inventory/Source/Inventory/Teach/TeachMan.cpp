// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "TeachMan.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/InputDeviceSubsystem.h"

#include "Inventory/Flags.h"
#include "JUtils/Input/JPInput.h"
#include "JUtils/Input/JUtilsInput.h"

#include "TeachTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogTeachMan, Log, Log)

namespace Inventory { namespace Teach {
	static const TCHAR* const Prefix = TEXT("Teach.");
}}

ATeachMan::ATeachMan():Super() {
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void ATeachMan::Init_Implementation() {
	UE_LOG(LogTeachMan, Log, TEXT("%hs"), __func__);
	Flags = UFlags::Instance(this);
	UE_CLOG(!Flags, LogTeachMan, Warning, TEXT("%hs Flag subsystem not found!"), __func__);
	
	UInputDeviceSubsystem* const Inputs = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();
	if (LIKELY(Inputs)) Inputs->OnInputHardwareDeviceChanged.AddDynamic(this, &ATeachMan::OnHardwareChanged);
	else UE_LOG(LogTeachMan, Log, TEXT("%hs can't get input subsystem"), __func__);

	const UEnhancedInputLocalPlayerSubsystem* const InputSub = UJUtilsInput::GetInputSub(this);
	UJPInput* const PInput = LIKELY(InputSub) ? Cast<UJPInput>(InputSub->GetPlayerInput()) : nullptr;
	if (LIKELY(PInput)) PInput->OnChange.AddUniqueDynamic(this, &ATeachMan::InputChanged);
}

void ATeachMan::DeInit_Implementation() {
	UInputDeviceSubsystem* const Inputs = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>();
	if (LIKELY(Inputs)) Inputs->OnInputHardwareDeviceChanged.RemoveAll(this);
	
	const UEnhancedInputLocalPlayerSubsystem* const InputSub = UJUtilsInput::GetInputSub(this);
	UJPInput* const PInput = LIKELY(InputSub) ? Cast<UJPInput>(InputSub->GetPlayerInput()) : nullptr;
	if (LIKELY(PInput)) PInput->OnChange.RemoveAll(this);

	DT = nullptr;
	DTs.Empty();
	Flags = nullptr;
	
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	
	World->GetTimerManager().ClearAllTimersForObject(this);
}

bool ATeachMan::Has(const FName& Id) const {
	const FName FN(Inventory::Teach::Prefix + Id.ToString());
	return LIKELY(Flags) && Flags->Has(FN);
}

void ATeachMan::Set_Implementation(const FName& Id) const {
	UE_LOG(LogTeachMan, Log, TEXT("%hs Id=%s"), __func__, *Id.ToString());
	const FName FN(Inventory::Teach::Prefix + Id.ToString());
	if (LIKELY(Flags)) Flags->Set(FN);
}

bool ATeachMan::Show_Implementation(const FName& Id) {
	UE_LOG(LogTeachMan, Log, TEXT("%hs Id=%s"), __func__, *Id.ToString());
	if (UNLIKELY(!CurrentId.IsNone())) {
		UE_LOG(LogTeachMan, Log, TEXT("%hs Busy."), __func__);
		return false;
	}

	if (UNLIKELY(Has(Id))) {
		UE_LOG(LogTeachMan, Log, TEXT("%hs User already saw this. Row=%s"), __func__, *Id.ToString());
		return true; // true since it's already shown.
	}

	return ShowNow(Id);
}

bool ATeachMan::ShowNow(const FName& Id) {
	if (UNLIKELY(Id.IsNone())) return false;
	UE_LOG(LogTeachMan, Log, TEXT("%hs Id=%s"), __func__, *Id.ToString());

	const UWorld* const W = GetWorld();

	if (UNLIKELY(!DT | !W)) {
		UE_LOG(LogTeachMan, Warning, TEXT("%hs DT, or World is not ok. DT=%s"), __func__, *GetNameSafe(DT));
		return false;
	}

	const FTeachRow* const pR = DT->FindRow<FTeachRow>(Id, "", false);
	if (UNLIKELY(!pR)) {
		UE_LOG(LogTeachMan, Log, TEXT("%hs Row not found. Row=%s"), __func__, *Id.ToString());
		return false;
	}

	FTeachRow NR = *pR;
	FFormatNamedArguments* const pArgs = KeyArgs.Find(Tgt);
	if (LIKELY(pArgs)) {
		NR.Text = FText::Format(pR->Text, *pArgs);
		UE_LOG(LogTeachMan, Log, TEXT("%hs Formatted =%s"), __func__, *NR.Text.ToString());
	} else {
		UE_LOG(LogTeachMan, Warning, TEXT("%hs Could not find keynames for target =%s"),
			__func__, *UEnum::GetValueAsString(Tgt));
	}
	
	Set(Id); // mark here as well to make logic easier.
	CurrentId = Id;
	LastTime = W->GetTimeSeconds();
	UE_LOG(LogTeachMan, Log, TEXT("%hs Time =%.3f"), __func__, pR->Time);
	OnShow.Broadcast(Id, NR); //*pR);

	const float T = (pR->Time) >0 ? (pR->Time) : Time;
	FTimerManager& Timer = W->GetTimerManager();
	Timer.ClearTimer(HShow);
	HShow.Invalidate();
	Timer.SetTimer(HShow, this, &ATeachMan::HideCurrent, T);
	return true;
}

void ATeachMan::Hide_Implementation(const FName Id) { // don't use ref here.
	// don't use ref, since we return it on OnHide. there's a slight chance it's a ref to CurrentId if called with HideCurrent (on timeout).

	UE_LOG(LogTeachMan, Log, TEXT("%hs Id=%s"), __func__, *Id.ToString());
	if (UNLIKELY(Id.IsNone() | !Flags)) {
		UE_LOG(LogTeachMan, Warning, TEXT("%hs I have nothing to hide. Or no flags."), __func__);
		return;
	}

	Set(Id); // allow to pre-emptively mark actions as learnt
	// and also hide the current one
	if (UNLIKELY(Id != CurrentId)) return;

	CurrentId = NAME_None;

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	World->GetTimerManager().ClearTimer(HShow); // in case it's called from somewhere else
	HShow.Invalidate();

	OnHide.Broadcast(Id);
}


void ATeachMan::SetDT(UDataTable* const InDT) {
	UE_LOG(LogTeachMan, Log, TEXT("%hs"), __func__);
	DT = InDT;
}

void ATeachMan::SetTarget(const EInputType Target) {
	// refresh the dialog if needed. shownow checks for currentid.isnone. and hide sets it to none.
	// so it's only valid while showing.
	Tgt = Target;
	ShowNow(CurrentId);
}

void ATeachMan::SetKeyNames(const EInputType Target, const TMap<FString, FText> Names) {
	FFormatNamedArguments Arg;
	for (const TTuple<FString, FText>& KV: Names) {
		Arg.Add(KV.Key, KV.Value);
	}

	KeyArgs.Add(Target, Arg); // replace the old one (can't use KeyArs[] or it will crash >:'[ )
}

void ATeachMan::OnHardwareChanged(const FPlatformUserId UserId, const FInputDeviceId DeviceId) {
	// todo this does not work
	// https://forums.unrealengine.com/t/enhanced-input-detect-gamepad-vs-keyboard-input/1231533/19?u=nande
	UE_LOG(LogTeachMan, Log, TEXT("%hs Input Device Changed %i"), __func__, DeviceId.GetId());
}

void ATeachMan::InputChanged(const EInputType NewTgt) {
	UE_LOG(LogTeachMan, Log, TEXT("%hs Input Changed Tgt=%s NewTgt=%s"), __func__,
		*UEnum::GetValueAsString(Tgt), *UEnum::GetValueAsString(NewTgt));
	SetTarget(NewTgt);
}
