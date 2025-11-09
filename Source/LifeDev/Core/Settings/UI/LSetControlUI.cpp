// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetControlUI.h"

#include "InputMappingContext.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#include "JButton.h"
#include "LInputSelector.h"
#include "JUtils/Misc/JUtilsSys.h"
#include "LifeDev/Core/Settings/LSysSettings.h"
#include "LifeDev/Game/Inventory/Teach/LTeachMan.h"

void ULSetControlUI::Apply_Implementation() {
	Super::Apply_Implementation();
	UEnhancedInputUserSettings* const Settings = UJUtilsSys::GetEInputSettings(this);
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
	ALTeachMan* const TeachMan = ALTeachMan::Instance(this);
	const ULSysSettings* const SysSettings = ULSysSettings::Get();
	if (UNLIKELY(!SysSettings | !TeachMan)) return;

	TMap<ETeachTarget, TMap<FString, FText>> Names;
	for (const TSoftObjectPtr<UInputMappingContext>& SIMC: SysSettings->IMCs) {
		const UInputMappingContext* const Imc = SIMC.LoadSynchronous();
		if (UNLIKELY(!Imc)) continue;

		TArray<FEnhancedActionKeyMapping> Mappings = Imc->GetMappings();
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
	}
	// TODO category doesn't give what i need.

	// todo fix will crash if names[] not set
	if (Names.Contains(ETeachTarget::DESK))
		TeachMan->SetKeyNames(ETeachTarget::DESK, Names[ETeachTarget::DESK]);
	if (Names.Contains(ETeachTarget::PAD))
		TeachMan->SetKeyNames(ETeachTarget::PAD, Names[ETeachTarget::PAD]);
}

void ULSetControlUI::NativeOnInitialized() {
	Super::NativeOnInitialized();

	// not sure if i *need* this. or if the load is async, which should be.
	UEnhancedInputUserSettings* const EISettings = UJUtilsSys::GetEInputSettings(this);
	const ULSysSettings* const SysSettings = ULSysSettings::Get();
	if (LIKELY(bool(EISettings) & bool(SysSettings))) {
		EISettings->LoadOrCreateSettings(GetWorld()->GetFirstLocalPlayerFromController());

		// force register the imcs so that the rebinding works well
		// i'm not super happy to do loadSynchronous, but this will get loaded at the start of each level (intro and game)

		for (const TSoftObjectPtr<UInputMappingContext>& SIMC: SysSettings->IMCs) {
			// const TSoftObjectPtr<UInputMappingContext> CtxDiags = TSoftObjectPtr<UInputMappingContext>(
				// FSoftObjectPath(TEXT("/Diags/Input/IMC_Dialogs.IMC_Dialogs")));
			const UInputMappingContext* const Imc = SIMC.LoadSynchronous();
			if (UNLIKELY(!Imc)) continue;
			// TArray<FEnhancedActionKeyMapping> Mappings = Imc->GetMappings();
			// Mappings[0].Action->ActionDescription
			// Mappings[0].Key.GetDisplayName();
			EISettings->RegisterInputMappingContext(Imc);

			// const TSoftObjectPtr<UInputMappingContext> CtxInv = TSoftObjectPtr<UInputMappingContext>(
				// FSoftObjectPath(TEXT("/Inventory/Input/IMC_Inventory.IMC_Inventory")));
			// EISettings->RegisterInputMappingContext(CtxInv.LoadSynchronous());

			// const TSoftObjectPtr<UInputMappingContext> CtxChar = TSoftObjectPtr<UInputMappingContext>(
				// FSoftObjectPath(TEXT("/Game/LifeDev/Game/Char/Input/IMC_Char.IMC_Char")));
			// EISettings->RegisterInputMappingContext(CtxChar.LoadSynchronous());

			// const TSoftObjectPtr<UInputMappingContext> CtxMenu = TSoftObjectPtr<UInputMappingContext>(
				// FSoftObjectPath(TEXT("/Game/LifeDev/Core/Settings/Input/Menu_IMC.Menu_IMC")));
			// EISettings->RegisterInputMappingContext(CtxMenu.LoadSynchronous());
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
	UJUtilsSys::ResetEInputMapsAll(this);
	Load();
}
