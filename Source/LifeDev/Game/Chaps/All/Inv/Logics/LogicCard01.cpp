// Copyright Jerónimo Barraco-Mármol

#include "LogicCard01.h"

#include "Diags/Diags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Core/Sentry.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ULogicCard01::ULogicCard01() {
	// since this is an "instance" class, this is fine.
	static ConstructorHelpers::FObjectFinder<UDataTable>
		CDT(TEXT("/Game/LifeDev/Game/Inventory/Logic/C1/C1_Entries"));
	DT = CDT.Object;
	// idea. be able to check the amount of each object (e.g. require X of an item, and Y of another, to give Z of another)
}

void ULogicCard01::Use_Implementation() {
	Super::Use_Implementation();

	static const FString NOk("Item.Use.C1.Ok.");
	static const FName NFail("Item.Use.C1.Fail"); // avoid conversion each time.

	if (UNLIKELY(!DT)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs datatable not set"), __func__);
		USentry::SAddMsg(this, "Logic Card 01 DT not set", ESentryLevel::Warning);
		return;
	}

	if (UNLIKELY(!Inv)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs no inventory."), __func__);
		USentry::SAddMsg(this, "Logic Card 01 no Inventory", ESentryLevel::Warning);
		return;
	}

	// find a group of items, replace with another
	bool HasAll = false;
	// apparently the optimal way to iterate
	for (TMap<FName, uint8*>::TConstIterator I(DT->GetRowMap().CreateConstIterator()); I; ++I) {
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

		if (LIKELY(Diags)) Diags->AddId(FName(NOk+I.Key().ToString()));
		return; // only one action at a time
	}

	if (LIKELY(Diags)) Diags->AddId(NFail);
}
