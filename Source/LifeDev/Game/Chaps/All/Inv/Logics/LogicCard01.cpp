// Copyright Jerónimo Barraco-Mármol

#include "LogicCard01.h"

#include "Inventory/Inventory.h"

ULogicCard01::ULogicCard01() {
	// Entries.Reserve(1);

	// since this is an "instance" class, this is fine.
	static ConstructorHelpers::FObjectFinder<UDataTable>
		CDT(TEXT("/Game/LifeDev/Game/Inventory/Logic/C1/C1_Entries"));
	DT = CDT.Object; // TODO test
	// TODO be able to check the amount of each object
}

void ULogicCard01::Use_Implementation() {
	Super::Use_Implementation();
	if (UNLIKELY(!DT)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs datatable not set"), __func__);
		return;
	}

	// find a group of items, replace with another
}

void ULogicCard01::BeginPlay_Implementation() {
	Super::BeginPlay_Implementation();
	if (UNLIKELY(!DT)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs datatable not set"), __func__);
		return;
	}

	// UJUtilsMisc::ReadTable<FCard01Entry>(DT, Entries);
}
