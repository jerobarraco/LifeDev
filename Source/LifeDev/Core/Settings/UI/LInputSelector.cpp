// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LInputSelector.h"

#include "JUtils/Misc/JUtilsSys.h"
#include "UserSettings/EnhancedInputUserSettings.h"

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
	// https://dev.epicgames.com/community/learning/tutorials/Vp69/unreal-engine-player-mappable-keys-using-enhanced-input
	FMapPlayerKeyArgs Args;
	Args.MappingName = InputName;
	Args.Slot = EPlayerMappableKeySlot::First;
	Args.NewKey = GetSelectedKey().Key;
	UEnhancedInputUserSettings* const Settings = UJUtilsSys::GetEInputSettings(this);
	// If you want to, you can additionally specify this mapping to only be applied to a certain hardware device or key profile
	//Args.ProfileId =
	//Args.HardwareDeviceId =
	FGameplayTagContainer FailureReason;
	if (LIKELY(Settings)) Settings->MapPlayerKey(Args, FailureReason);
}

void ULInputSelector::Load() {
	const UEnhancedPlayerMappableKeyProfile* const Profile = UJUtilsSys::GetEInputProfile(this);
	TArray<FKey> Keys;
	Profile->GetMappedKeysInRow(InputName, Keys);
	if (Keys.Num()>0) SetSelectedKey(Keys[0]);

	SetAllowGamepadKeys(IsMapGP());
}

bool ULInputSelector::IsMapGP() const {
	return InputName.ToString().EndsWith("_GP");
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
	UJUtilsSys::ResetEInputMap(this, InputName);
}

void ULInputSelector::OnWidgetRebuilt() {
	Super::OnWidgetRebuilt();
	ResetStyle();
}

void ULInputSelector::KeySelected(const FInputChord Key) { // can't be ref due to how the delegate is set
	OnKeySelectedPlus.Broadcast(this, Key);
}
