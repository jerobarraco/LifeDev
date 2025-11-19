// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LInputSelector.h"

#include "UserSettings/EnhancedInputUserSettings.h"

#include "JUtils/Misc/JUtilsInput.h"

ULInputSelector::ULInputSelector():Super() {
	// ATM the enhanced input will make the key unusable when i rebind it
	// Super::SetIsEnabled(false);

	// this is so good 
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CSB(TEXT("/Game/LifeDev/Core/UI/Btns/LButtonSmall_S"));
	StyleBtn = CSB.Object;
	static ConstructorHelpers::FObjectFinder<USlateWidgetStyleAsset>
		CST(TEXT("/Game/LifeDev/Core/UI/Btns/LTextSmall_S"));
	StyleText = CST.Object;

	SetNoKeySpecifiedText(NSLOCTEXT("InputSelector", "NoKey", "[None]"));
	SetKeySelectionText(NSLOCTEXT("InputSelector", "Waiting", "[Waiting...]"));
	SetAllowModifierKeys(false);
	EscapeKeys.Add(EKeys::Escape);
	EscapeKeys.Add(EKeys::Gamepad_Special_Right);
	
	// SetToolTipText(FText::FromString(TEXT("Sorry. Rebinding is not available at the moment.")));
}

void ULInputSelector::Init() {
	OnKeySelected.AddUniqueDynamic(this, &ULInputSelector::KeySelected);
}

void ULInputSelector::DeInit() {
	OnKeySelected.RemoveAll(this);
}

void ULInputSelector::Apply() {
	if (UNLIKELY(InputName.IsNone())) return;

	UE_LOG(LogTemp, Log, TEXT("LInputSelector::%hs Name=%s"), __func__, *InputName.ToString());

	// https://dev.epicgames.com/community/learning/tutorials/Vp69/unreal-engine-player-mappable-keys-using-enhanced-input
	FMapPlayerKeyArgs Args;
	Args.MappingName = InputName;
	Args.Slot = EPlayerMappableKeySlot::First;
	Args.NewKey = GetSelectedKey().Key;
	UEnhancedInputUserSettings* const Settings = UJUtilsInput::GetInputSettings(this);
	// If you want to, you can additionally specify this mapping to only be applied to a certain hardware device or key profile
	//Args.ProfileId =
	//Args.HardwareDeviceId =
	FGameplayTagContainer FailureReason;
	if (LIKELY(Settings)) Settings->MapPlayerKey(Args, FailureReason);
}

void ULInputSelector::Load() {
	UE_LOG(LogTemp, Log, TEXT("LInputSelector::%hs Name=%s"), __func__, *InputName.ToString());

	const UEnhancedPlayerMappableKeyProfile* const Profile = UJUtilsInput::GetInputProfile(this);
	TArray<FKey> Keys;
	Profile->GetMappedKeysInRow(InputName, Keys);
	if (Keys.Num()>0) SetSelectedKey(Keys[0]);

	SetAllowGamepadKeys(IsMapGP());
}

bool ULInputSelector::IsMapGP() const {
	// const UEnhancedPlayerMappableKeyProfile* const Profile = UJUtilsSys::GetEInputProfile(this);
	// TArray<FKey> Keys;
	// Profile->GetMappedKeysInRow(InputName, Keys);
	// Keys[0].IsGamepadKey(); or JUtils inputtarget
	// what i want to know is if the slot is for gamepads, not if the actual key bound.

	return InputName.ToString().EndsWith(".GP");
}

void ULInputSelector::ResetStyle() {
	if (StyleBtn) {
		const FButtonStyle* const Style = StyleBtn->GetStyle<FButtonStyle>();
		if (LIKELY(Style)) SetButtonStyle(*Style);
	}

	if (StyleText) {
		const FTextBlockStyle* const Style = StyleText->GetStyle<FTextBlockStyle>();
		if (LIKELY(Style)) SetTextStyle(*Style);
	}
}

void ULInputSelector::SetDefault() const {
	UJUtilsInput::ResetInputMap(this, InputName);
}

void ULInputSelector::OnWidgetRebuilt() {
	Super::OnWidgetRebuilt();
	ResetStyle();
}

void ULInputSelector::KeySelected(const FInputChord Key) { // can't be ref due to how the delegate is set
	OnKeySelectedPlus.Broadcast(this, Key);
}
