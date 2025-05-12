// Copyright Jerónimo Barraco-Mármol

#include "SpotI00.h"

#include "Diags/Diags.h"
#include "Interact/InteractTypes.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ASpotI00::ASpotI00():Super() {
	Texts = {
		FText(NSLOCTEXT("Chap01", "Spot00.DropHere", "Drop clothes here")), 
		FText(NSLOCTEXT("Chap01", "Spot00.Full", "All done")),
	};
	Items = {
		// two shirts and a boxer, and a sock
		FName("C1C00"), FName("C1C01"), FName("C1C02"), FName("C1C04")
	};
}

// TODO this is not used anymore. to be deleted. not wrapped in a ifdef to not make it harder to find
EItemUseResult ASpotI00::TryUseItem_Implementation(const FName& Name) {
	if (Name == LDConsts::Items::Bra) { // custom dialog for the bra
		bool Handled = false;
		if (LIKELY(Diags)) Handled = Diags->AddId("IS_C0_C03");
		return Handled ? EItemUseResult::BAD_HANDLED : EItemUseResult::BAD_TARGET;
	}

	return Super::TryUseItem_Implementation(Name);
}
