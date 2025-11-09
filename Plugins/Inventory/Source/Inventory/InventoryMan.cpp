// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "InventoryMan.h"

// these two are needed anyway otherwise it won't compile
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

#include "JUtils/Misc/JUtilsInput.h"

#include "InventoryUI.h"
#include "Inventory.h"

AInventoryMan::AInventoryMan(): Super() {
	PrimaryActorTick.bCanEverTick = false;
	Super::SetActorTickEnabled(false);

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMapping(TEXT("/Inventory/Input/IMC_Inventory"));
	Mapping = DefaultMapping.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionOpen(TEXT("/Inventory/Input/IA_Open"));
	ActionOpen = CActionOpen.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionSelect(TEXT("/Inventory/Input/IA_Select"));
	ActionSelect = CActionSelect.Object;
	UIClass = UInventoryUI::StaticClass();
}

void AInventoryMan::Init_Implementation() {}

void AInventoryMan::DeInit_Implementation() {
	Hide();
	if (LIKELY(IsValid(UI))) {
		UI->RemoveFromParent();
		UI->OnDone.RemoveAll(this);
	}
	UI = nullptr;

	if (LIKELY(IsValid(Inventory))) {
		Inventory->OnMod.RemoveAll(this);
		Inventory->OnSelected.RemoveAll(this);
		Inventory->OnUsed.RemoveAll(this);
		Inventory->OnCold.RemoveAll(this);
	}
	Inventory = nullptr;

	UEnhancedInputComponent* const Input = UJUtilsInput::GetInput(this);
	if (LIKELY(IsValid(Input))) Input->ClearBindingsForObject(this);

	UJUtilsInput::ToggleContext(this, Mapping, InputPrio, false);
}

void AInventoryMan::ActOpen() {
	Show();
}

void AInventoryMan::ActSelect(const FInputActionValue& InputActionValue) {
	const bool Next = InputActionValue.GetMagnitude() > 0;
	const FName& NextKey = Inventory->GetNextKey(Next);
	if (UNLIKELY(NextKey.IsNone())) return;

	Inventory->SetSelected(NextKey);
}

void AInventoryMan::SetVisible(const bool Vis) {
	if (Vis) Show();
	else Hide();
}

void AInventoryMan::Show() {
	if (UNLIKELY(IsShowing | !IsShowEnabled)) return;

	IsShowing = true;
	if (LIKELY(IsValid(UI))) UI->Show();
}

void AInventoryMan::Hide() {
	if (LIKELY(IsValid(UI))) UI->Hide();
	IsShowing = false;
}

void AInventoryMan::SetSelected(const FName& Name) {
	if (LIKELY(IsValid(UI))) UI->SetSelected(Name);
}

void AInventoryMan::SetItemMod(const FName& Name, const int32 Diff, const FItem& Item) {
	if (LIKELY(IsValid(UI))) UI->SetItemMod(Name, Diff, Item);
}

void AInventoryMan::SetItemCold(const FName& Name) {
	if (LIKELY(IsValid(UI))) UI->SetItemCold(Name);
}

void AInventoryMan::SetItemUsed(const FName& Name) {
	if (LIKELY(IsValid(UI))) UI->SetItemUsed(Name);
}

void AInventoryMan::BeginPlay() {
	Super::BeginPlay();

	// bind the action
	UWorld* const World = GetWorld();
	if (UNLIKELY(!IsValid(World))) return;
	
	UJUtilsInput::ToggleContext(this, Mapping, InputPrio, true);
	if (ActionOpen) {
		UEnhancedInputComponent* const Input = UJUtilsInput::GetInput(this);
		if (LIKELY(IsValid(Input))) {
			Input->BindAction<AInventoryMan>(
				ActionOpen, ETriggerEvent::Triggered, this, &AInventoryMan::ActOpen);
			Input->BindAction<AInventoryMan>(
				ActionSelect, ETriggerEvent::Triggered, this, &AInventoryMan::ActSelect);
		}
	}
	
	UClass* const Class = UIClass.Get();
	if (LIKELY(IsValid(Class))) {
		UI = CreateWidget<UInventoryUI>(World, Class);
		if (LIKELY(IsValid(UI))) {
			UI->AddToViewport(ZOrder);
			Hide();
			UI->OnDone.AddUniqueDynamic(this, &AInventoryMan::UIDone);
		}
	}
	
	Inventory = World->GetSubsystem<UInventory>();
	if (UNLIKELY(!Inventory)) return;

	Inventory->OnSelected.AddUniqueDynamic(this, &AInventoryMan::SetSelected);
	Inventory->OnMod.AddUniqueDynamic(this, &AInventoryMan::SetItemMod);
	Inventory->OnCold.AddUniqueDynamic(this, &AInventoryMan::SetItemCold);
	Inventory->OnUsed.AddUniqueDynamic(this, &AInventoryMan::SetItemUsed);
}

void AInventoryMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	DeInit();

	Super::EndPlay(EndPlayReason);
}

void AInventoryMan::UIDone() {
	Hide();
}
