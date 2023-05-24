// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "InventoryManager.h"

// these two are needed anyway otherwise it wont compile
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Inventory.h"

#include "InventoryUI.h"
#include "JUtils/JMiscUtils.h"

AInventoryManager::AInventoryManager():Super(){
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMapping(TEXT("/Inventory/Input/IMC_Inventory"));
	Mapping = DefaultMapping.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionOpen(TEXT("/Inventory/Input/IA_Open"));
	ActionOpen = CActionOpen.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionSelect(TEXT("/Inventory/Input/IA_Select"));
	ActionSelect = CActionSelect.Object;
	
	UIClass = UInventoryUI::StaticClass();
}

void AInventoryManager::Init() {}
void AInventoryManager::DeInit() {
	Hide();
	if (IsValid(UI)) {
		UI->RemoveFromParent();
		UI->OnDone.RemoveAll(this);
	}
	UI = nullptr;
	UJMiscUtils::ToggleMapping(Mapping, InputPrio, false, GetWorld());
}

void AInventoryManager::ActOpen() {
	if (IsShowing) return;
	IsShowing = true;
	if (IsValid(UI)) {
		UI->Show();
	}
}

void AInventoryManager::ActSelect(const FInputActionValue& InputActionValue) {
	const bool Next = InputActionValue.GetMagnitude() > 0;
	const TMap<FName, int32>& Items = Inventory->GetItems();
	// nothing to do
	if (Items.Num() <= 0 ) return;
	
	// This code has a lot of similarities with the selected change on Inventory.Mod
	TArray<FName> Keys;
	Items.GetKeys(Keys);
	const FName& Selected = Inventory->GetSelected();
	const int32 Index = Keys.Find(Selected);
	int32 NewIndex = Index +  (Next ? 1 : -1);
	NewIndex = FMath::Clamp(NewIndex, 0, Keys.Num() -1);
	Inventory->SetSelected(Keys[NewIndex]);
}

void AInventoryManager::Hide() {
	if (IsValid(UI)) {
		UI->Hide();
	}
	IsShowing = false;
}

void AInventoryManager::SetSelected(const FName& Name) {
	if (IsValid(UI)) {
		UI->SetSelected(Name);
	}
}

void AInventoryManager::BeginPlay() {
	Super::BeginPlay();

	// bind the action
	UWorld* const World = GetWorld();
	if (!IsValid(World)) return;
	
	if (ActionOpen) {
		UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(World->GetFirstPlayerController()->InputComponent);
		if (IsValid(Input)) {
			Input->BindAction<AInventoryManager>(
				ActionOpen, ETriggerEvent::Triggered, this, &AInventoryManager::ActOpen);
			Input->BindAction<AInventoryManager>(
				ActionSelect, ETriggerEvent::Triggered, this, &AInventoryManager::ActSelect);
		}
	}
	
	UClass* const Class = UIClass.Get();
	if (IsValid(Class)) {
		UI = NewObject<UInventoryUI>(this, Class);
		if (IsValid(UI)) {
			UI->AddToViewport();
			Hide();
			UI->OnDone.AddUniqueDynamic(this, &AInventoryManager::UIDone);
		}
	}

	UJMiscUtils::ToggleMapping(Mapping, InputPrio, true, GetWorld());

	Inventory = World->GetSubsystem<UInventory>();
	Inventory->OnSelected.AddUniqueDynamic(this, &AInventoryManager::SetSelected);
}

void AInventoryManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	DeInit();
}

void AInventoryManager::UIDone() {
	Hide();
}
