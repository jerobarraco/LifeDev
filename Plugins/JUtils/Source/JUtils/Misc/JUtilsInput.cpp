// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "JUtilsInput.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UJUtilsInput::ToggleContext(const UObject* const O,
	const UInputMappingContext* const Ctx, const int32 Prio, const bool Enable, const bool WSetting) {
	UEnhancedInputLocalPlayerSubsystem* const Subsystem = GetInputSub(O);
	if (UNLIKELY(!IsValid(Subsystem))) return;

	FModifyContextOptions Op;
	Op.bNotifyUserSettings = WSetting; // todo not sure i need this.
	UEnhancedInputUserSettings* const Settings = Subsystem->GetUserSettings();
	if (Enable) {
		Subsystem->AddMappingContext(Ctx, Prio, Op);
		// attempt at making this work with user settings
		if (LIKELY(Settings)) Settings->RegisterInputMappingContext(Ctx);
	}
	else {
		Subsystem->RemoveMappingContext(Ctx, Op);
		if (LIKELY(Settings)) Settings->UnregisterInputMappingContext(Ctx);
	}
}

APlayerController* UJUtilsInput::GetFirstLocalPlayerController(const UObject* const O) {
	const UWorld* const W = O ? O->GetWorld():nullptr;
	if (UNLIKELY(!W)) return nullptr;

	const UGameInstance* const Instance = W->GetGameInstance();
	if (UNLIKELY(!Instance)) return nullptr;

	return Instance->GetFirstLocalPlayerController(W);
}

UEnhancedInputComponent* UJUtilsInput::GetInput(const UObject* const O) {
	const APlayerController* const Controller = GetFirstLocalPlayerController(O);
	if (UNLIKELY(!Controller)) return nullptr;

	UEnhancedInputComponent* const Input =
		Cast<UEnhancedInputComponent>(Controller->InputComponent);
	return Input;
}

UEnhancedInputLocalPlayerSubsystem* UJUtilsInput::GetInputSub(const UObject* const O) {
	const UWorld* const W = LIKELY(O) ? O->GetWorld() : nullptr;
	const ULocalPlayer* const Player = LIKELY(W) ? W->GetFirstLocalPlayerFromController() : nullptr;
	UEnhancedInputLocalPlayerSubsystem* const Subsystem =
		LIKELY(Player) ? ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player) : nullptr;
	UE_CLOG(!Subsystem, LogTemp, Warning, TEXT("%hs Could not get the current UEnhancedInputLocalPlayerSubsystem"), __func__);
	return Subsystem;
}

UEnhancedInputUserSettings* UJUtilsInput::GetInputSettings(const UObject* const O) {
	const UEnhancedInputLocalPlayerSubsystem* const Subsystem = GetInputSub(O);
	UEnhancedInputUserSettings* const Settings = LIKELY(Subsystem) ? Subsystem->GetUserSettings() : nullptr;
	UE_CLOG(!Settings, LogTemp, Warning, TEXT("%hs Could not get the EnhancedInputUserSettings"), __func__);
	return Settings;
}

UEnhancedPlayerMappableKeyProfile* UJUtilsInput::GetInputProfile(const UObject* const O) {
	const UEnhancedInputUserSettings* const Settings = GetInputSettings(O);
	
	UEnhancedPlayerMappableKeyProfile* const Profile = LIKELY(Settings) ? Settings->GetActiveKeyProfile() : nullptr;
	UE_CLOG(!Profile, LogTemp, Warning, TEXT("%hs Could not get the current EnhancedPlayerMappableKeyProfile"), __func__);
	return Profile;
}

void UJUtilsInput::ResetInputMapsAll(const UObject* const O) {
	// https://forums.unrealengine.com/t/get-enhanced-input-local-player-subsystem-in-c/1732524/2
	UEnhancedPlayerMappableKeyProfile* const Profile = GetInputProfile(O);
	if (LIKELY(Profile)) Profile->ResetToDefault();
}

void UJUtilsInput::ResetInputMap(const UObject* const O, const FName N) {
	if (UNLIKELY(N.IsNone() | !O)) return; // !O is cheap so put it there too.

	UEnhancedPlayerMappableKeyProfile* const Profile = GetInputProfile(O);
	if (LIKELY(Profile)) Profile->ResetMappingToDefault(N);
}

EInputType UJUtilsInput::GetKeyType(const FKey& Key) {
	// Key.GetMenuCategory() this also hints to the target. but i'm unsure how accurate it is
	if (Key.IsGamepadKey()) return EInputType::PAD;
	if (Key.IsTouch()) return EInputType::TOUCH;
	return EInputType::KBM;
}