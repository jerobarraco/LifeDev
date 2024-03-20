// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "InventoryManager.h"

// these two are needed anyway otherwise it wont compile
#include "CItemView.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Inventory.h"

#include "InventoryUI.h"
#include "JUtils/JMiscUtils.h"

AInventoryManager::AInventoryManager():Super(){
	PrimaryActorTick.bCanEverTick = false;
	Super::SetActorTickEnabled(false);

	View = CreateDefaultSubobject<UCItemView>(TEXT("View"));

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
	if (IsValid(Inventory)) {
		Inventory->OnMod.RemoveAll(this);
		Inventory->OnSelected.RemoveAll(this);
		Inventory->OnUsed.RemoveAll(this);
		Inventory->OnCold.RemoveAll(this);
	}
	Inventory = nullptr;
	UJMiscUtils::ToggleMapping(this, Mapping, InputPrio, false);
}

void AInventoryManager::ActOpen() {
	Show();
}

void AInventoryManager::ActSelect(const FInputActionValue& InputActionValue) {
	const bool Next = InputActionValue.GetMagnitude() > 0;
	const FName& NextKey = Inventory->GetNextKey(Next);
	if (NextKey.IsNone()) return;

	Inventory->SetSelected(NextKey);
}

void AInventoryManager::SetVisible(bool Vis) {
	if (Vis) {
		Show();
	} else {
		Hide();
	}
}

void AInventoryManager::Show() {
	if (IsShowing) return;
	IsShowing = true;
	if (IsValid(UI)) {
		UI->Show();
	}
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

void AInventoryManager::SetItemMod(const FName& Name, int32 Diff, const FItem& Item) {
	if (IsValid(UI)) {
		UI->SetItemMod(Name, Diff, Item);
	}
}

void AInventoryManager::SetItemCold(const FName& Name) {
	if (IsValid(UI)) {
		UI->SetItemCold(Name);
	}
}

void AInventoryManager::SetItemUsed(const FName& Name) {
	if (IsValid(UI)) {
		UI->SetItemUsed(Name);
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
		UI = CreateWidget<UInventoryUI>(World, Class);
		if (IsValid(UI)) {
			UI->AddToViewport(ZOrder);
			Hide();
			UI->OnDone.AddUniqueDynamic(this, &AInventoryManager::UIDone);
		}
	}

	UJMiscUtils::ToggleMapping(this, Mapping, InputPrio, true);

	Inventory = World->GetSubsystem<UInventory>();
	Inventory->OnSelected.AddUniqueDynamic(this, &AInventoryManager::SetSelected);
	Inventory->OnMod.AddUniqueDynamic(this, &AInventoryManager::SetItemMod);
	Inventory->OnCold.AddUniqueDynamic(this, &AInventoryManager::SetItemCold);
	Inventory->OnUsed.AddUniqueDynamic(this, &AInventoryManager::SetItemUsed);
}

void AInventoryManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	DeInit();
}

void AInventoryManager::UIDone() {
	Hide();
}
