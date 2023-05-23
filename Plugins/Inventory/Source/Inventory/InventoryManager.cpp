// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "InventoryManager.h"

// these two are needed anyway otherwise it wont compile
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

#include "InventoryUI.h"
#include "JUtils/JMiscUtils.h"

AInventoryManager::AInventoryManager():Super(){
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMapping(TEXT("/Inventory/Input/IMC_Inventory"));
	Mapping = DefaultMapping.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> CActionOpen(TEXT("/Inventory/Input/IA_Open"));
	ActionOpen = CActionOpen.Object;
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

void AInventoryManager::BeginPlay() {
	Super::BeginPlay();

	// bind the action
	UWorld* const World = GetWorld();
	if (ActionOpen) {
		UEnhancedInputComponent* const Input = Cast<UEnhancedInputComponent>(World->GetFirstPlayerController()->InputComponent);
		if (IsValid(Input)) {
			Input->BindAction<AInventoryManager>(
				ActionOpen, ETriggerEvent::Triggered, this, &AInventoryManager::Show);
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
}

void AInventoryManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	DeInit();
}

void AInventoryManager::UIDone() {
	Hide();
}
