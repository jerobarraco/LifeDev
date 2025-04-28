// Copyright Jerónimo Barraco-Mármol

#include "LogicCard01.h"

#include "JUtils/Misc/JUtilsMisc.h"

ULogicCard01::ULogicCard01() {
	Entries.Reserve(1);

	// since this is an "instance" class, this is fine.
	static ConstructorHelpers::FObjectFinder<UDataTable>
		CDT(TEXT("/Game/LifeDev/...."));
	DT = CDT.Object; // TODO
}

void ULogicCard01::Use_Implementation() {
	Super::Use_Implementation();
	// find a group of items, replace with another
}

void ULogicCard01::BeginPlay_Implementation() {
	Super::BeginPlay_Implementation();
	if (UNLIKELY(!DT)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs datatable not set"), __func__);
	}

	UJUtilsMisc::ReadTable<FCard01Entry>(DT, Entries);
}
