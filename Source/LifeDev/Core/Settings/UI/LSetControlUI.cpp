// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSetControlUI.h"

#include "InputMappingContext.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#include "JButton.h"
#include "LInputSelector.h"
#include "JUtils/Misc/JUtilsSys.h"

void ULSetControlUI::Apply_Implementation() {
	Super::Apply_Implementation();
	UEnhancedInputUserSettings* const Settings = UJUtilsSys::GetEInputSettings(this);
	if (LIKELY(Settings)) Settings->AsyncSaveSettings();
	
	for (const TObjectPtr<ULInputSelector>& S: Selectors) {
		if (UNLIKELY(!S)) continue;
		S->Apply();
	}
}

void ULSetControlUI::Load_Implementation() {
	Super::Load_Implementation();
	for (const TObjectPtr<ULInputSelector>& S: Selectors) {
		if (UNLIKELY(!S)) continue;
		S->Load();
	}
}

void ULSetControlUI::NativeOnInitialized() {
	Super::NativeOnInitialized();

	// not sure if i *need* this. or if the load is async, which should be.
	UEnhancedInputUserSettings* const EISettings = UJUtilsSys::GetEInputSettings(this);
	if (EISettings) EISettings->LoadOrCreateSettings(GetWorld()->GetFirstLocalPlayerFromController());
	
	// force register the imcs so that the rebinding works well
	// i'm not super happy to do loadSynchronous, but this will get loaded at the start of each level (intro and game)
	const TSoftObjectPtr<UInputMappingContext> CtxDiags = TSoftObjectPtr<UInputMappingContext>(
		FSoftObjectPath(TEXT("/Diags/Input/IMC_Dialogs.IMC_Dialogs")));
	EISettings->RegisterInputMappingContext(CtxDiags.LoadSynchronous());

	const TSoftObjectPtr<UInputMappingContext> CtxInv = TSoftObjectPtr<UInputMappingContext>(
		FSoftObjectPath(TEXT("/Inventory/Input/IMC_Inventory.IMC_Inventory")));
	EISettings->RegisterInputMappingContext(CtxInv.LoadSynchronous());

	const TSoftObjectPtr<UInputMappingContext> CtxChar = TSoftObjectPtr<UInputMappingContext>(
		FSoftObjectPath(TEXT("/Game/LifeDev/Game/Char/Input/IMC_Char.IMC_Char")));
	EISettings->RegisterInputMappingContext(CtxChar.LoadSynchronous());

	const TSoftObjectPtr<UInputMappingContext> CtxMenu = TSoftObjectPtr<UInputMappingContext>(
		FSoftObjectPath(TEXT("/Game/LifeDev/Core/Settings/Input/Menu_IMC.Menu_IMC")));
	EISettings->RegisterInputMappingContext(CtxMenu.LoadSynchronous());

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
