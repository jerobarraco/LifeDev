// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "ItemLogic.h"

#include "Engine/World.h"

UItemLogic::UItemLogic():Super() {
	// https://forums.unrealengine.com/t/uobject-begin-play-equivalent/472891/12
	// FWorldDelegates::OnPostWorldCreation.AddUObject(this, &UItemLogic::BeginPlay);
}

void UItemLogic::BeginPlay_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("UItemLogic::BeginPlay_Implementation"));
}

void UItemLogic::PostInitProperties() {
	// https://forums.unrealengine.com/t/uobject-begin-play-equivalent/472891/2?u=nande
	UObject::PostInitProperties();

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	// defer for next tick. Not sure if this is needed. but it might.
	// it will avoid issues like in the Inventory where the name is set after calling NewObject
	// Plugins\Inventory\Source\Inventory\Inventory.cpp:371
	World->GetTimerManager().SetTimerForNextTick(this, &UItemLogic::BeginPlay);
}
