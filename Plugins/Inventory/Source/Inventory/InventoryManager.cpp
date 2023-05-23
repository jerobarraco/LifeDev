// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "InventoryManager.h"

// these two are needed anyway otherwise it wont compile
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

AInventoryManager::AInventoryManager():Super(){
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);

	// UIClass = UDialogUI::StaticClass();
}

void AInventoryManager::Init() {}
void AInventoryManager::DeInit() {}
void AInventoryManager::Show() {}
void AInventoryManager::Hide() {}

void AInventoryManager::BeginPlay() {
	Super::BeginPlay();
}

void AInventoryManager::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}

void AInventoryManager::UIDone() {}
