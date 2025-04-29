// Copyright Jerónimo Barraco-Mármol

#include "LogicCard01.h"

#include "Diags/Diags.h"
#include "Inventory/Inventory.h"

ULogicCard01::ULogicCard01() {
	// since this is an "instance" class, this is fine.
	static ConstructorHelpers::FObjectFinder<UDataTable>
		CDT(TEXT("/Game/LifeDev/Game/Inventory/Logic/C1/C1_Entries"));
	DT = CDT.Object;
	// TODO be able to check the amount of each object (e.g. require X of an item, and Y of another, to give Z of another)
}

void ULogicCard01::Use_Implementation() {
	Super::Use_Implementation();
	
	static const FString NOk("Item.Use.C1.Ok.");
	static const FName NFail("Item.Use.C1.Fail"); // avoid conversion each time.
	if (UNLIKELY(!DT)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs datatable not set"), __func__);
		return;
	}
	if (UNLIKELY(!Inv)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs no inventory."), __func__);
		return;
	}

	// find a group of items, replace with another
	bool HasAll = false;
	for (TMap<FName, uint8*>::TConstIterator I(DT->GetRowMap().CreateConstIterator()); I; ++I ) {
		const FCard01Entry* pE = reinterpret_cast<FCard01Entry*>(I.Value());
		if (UNLIKELY(!pE)) continue;

		HasAll = true;
		for (const FName& F: pE->From) {
			if (Inv->Has(F)) continue;
			HasAll = false;
			break;
		}

		if (LIKELY(!HasAll)) continue; 

		if (pE->Consume)
			for (const FName& F: pE->From)
				Inv->Mod(F, -1);

		for (const FName& F: pE->To)
			Inv->Mod(F, 1);

		Diags->AddId(FName(NOk+I.Key().ToString()));
		return;
	}

	Diags->AddId(NFail);
}
