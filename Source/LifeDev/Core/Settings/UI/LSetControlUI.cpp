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

	
	// find a better place and way, this crashes the game. maybe on the intro game mode
	// these two are necessary so that the inputselectors in the intro 
	const TSoftObjectPtr<UInputMappingContext> CtxDiags = TSoftObjectPtr<UInputMappingContext>(
		FSoftObjectPath(TEXT("/Diags/Input/IMC_Dialogs.IMC_Dialogs")));
	EISettings->RegisterInputMappingContext(CtxDiags.LoadSynchronous());

	const TSoftObjectPtr<UInputMappingContext> CtxInv = TSoftObjectPtr<UInputMappingContext>(
		FSoftObjectPath(TEXT("/Inventory/Input/IMC_Inventory.IMC_Inventory")));
	EISettings->RegisterInputMappingContext(CtxInv.LoadSynchronous());

	const TSoftObjectPtr<UInputMappingContext> CtxChar = TSoftObjectPtr<UInputMappingContext>(
		FSoftObjectPath(TEXT("/Game/LifeDev/Game/Char/Input/IMC_Char.IMC_Char")));
	EISettings->RegisterInputMappingContext(CtxChar.LoadSynchronous());

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
