// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetControlUI.h"

#include "UserSettings/EnhancedInputUserSettings.h"

#include "JButton.h"
#include "LInputSelector.h"
#include "JUtils/Misc/JUtilsInput.h"

#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Game/Inventory/Teach/LTeachMan.h"

void ULSetControlUI::Apply_Implementation() {
	Super::Apply_Implementation();
	UEnhancedInputUserSettings* const Settings = UJUtilsInput::GetEInputSettings(this);
	if (LIKELY(Settings)) {
		Settings->ApplySettings();
		Settings->AsyncSaveSettings();
	}

	for (const TObjectPtr<ULInputSelector>& S: Selectors) {
		if (UNLIKELY(!S)) continue;

		S->Apply();
	}
	
	ApplyKeyNames();
}

void ULSetControlUI::Load_Implementation() {
	Super::Load_Implementation();
	for (const TObjectPtr<ULInputSelector>& S: Selectors) {
		if (UNLIKELY(!S)) continue;
		S->Load();
	}
}

void ULSetControlUI::ApplyKeyNames() {
	// TODO move this somewhere else. i need to control when it's called and call from outside
	ALTeachMan* const TeachMan = ALTeachMan::Instance(this);
	if (UNLIKELY(!TeachMan)) return;

	TeachMan->SetCurrentKeyNames();
}

void ULSetControlUI::NativeOnInitialized() {
	Super::NativeOnInitialized();

	// this is necessary for rebinding. but also is a pre-requisite for current keys. so i put them together here.
	// though probably belong somewhere else.

	// not sure if i *need* this. or if the load is async, which should be. but i'm relying it on being sync.
	UEnhancedInputUserSettings* const EISettings = UJUtilsInput::GetEInputSettings(this);
	const ULSysSettings* const SysSettings = ULSysSettings::Get();
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	if (LIKELY(bool(EISettings) & bool(SysSettings))) {
		EISettings->LoadOrCreateSettings(W->GetFirstLocalPlayerFromController());

		// force register the imcs so that the rebinding works well
		// i'm not super happy to do loadSynchronous, but this will get loaded at the start of each level (intro and game)

		for (const TSoftObjectPtr<UInputMappingContext>& SIMC: SysSettings->IMCs) {
			const UInputMappingContext* const Imc = SIMC.LoadSynchronous();
			if (UNLIKELY(!Imc)) continue;

			EISettings->RegisterInputMappingContext(Imc);
		}
	}
	ApplyKeyNames();

	if (LIKELY(BDefaults))
		BDefaults->OnClick.AddUniqueDynamic(this, &ULSetControlUI::SetDefaults);

	for (const TObjectPtr<ULInputSelector>& S: Selectors) {
		if (UNLIKELY(!S)) continue;
		S->Init();
	}
}

void ULSetControlUI::NativeDestruct() {
	if (LIKELY(BDefaults))
		BDefaults->OnClick.RemoveAll(this);
	
	for (const TObjectPtr<ULInputSelector>& S: Selectors) {
		if (UNLIKELY(!S)) continue;
		S->DeInit();
	}
	Super::NativeDestruct();
}

void ULSetControlUI::SetDefaults(const int32 Id) {
	UJUtilsInput::ResetEInputMapsAll(this);
	Load();
}

/*
for (const FEnhancedActionKeyMapping& M: Mappings) {
	const FKey& Key = M.Key;
	const FName& FNamePre = Key.GetFName();
	if (FNamePre.IsNone()) continue; // means the key is unmappable
	// const FName& Name = M.Key.GetMenuCategory(); // the category doesn't give what i want.
	// it actually gives the target "gamepad" "keyboard" etc
	
	// derive the generic name from the name. remove the last _gp _kb
	const FString& NamePre = FNamePre.ToString();
	FString NameLeft, NameRight;
	NamePre.Split("_", &NameLeft, &NameRight, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	const FName Name(NameLeft);
	if (Name.IsNone()) continue;

	const FText& Text = Key.GetDisplayName(); // i hope this works. it binds to the key.
	const ETeachTarget Tgt = UJUtilsSys::GetKeyTarget(Key);
	TMap<FString, FText>& Map = Names.FindOrAdd(Tgt);
	Map.Add(Name.ToString(), Text);
	UE_LOG(LogTemp, Log, TEXT("%hs Added key text=%s name=%s tgt=%s"), __func__,
		*Text.ToString(), *Name.ToString(), *UEnum::GetValueAsString(Tgt));
}
// TODO category doesn't give what i need.
*/