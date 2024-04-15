// Copyright Jerónimo Barraco-Mármol

#include "SpotI00.h"

#include "Diags/Diags.h"
#include "Interact/InteractTypes.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ASpotI00::ASpotI00():Super() {
	DropDlg = TEXT("IS_C0D");
	TriggerDlg = "IS_C0T"; // TODO use a IS_C0T* (note the random)
	LockedDlg = "IS_C0L";
	LockedFullDlg = "IS_C0TB";
	Items = {
		// two shirts and a boxer, and a sock
		FName("C1C00"), FName("C1C01"), FName("C1C02"), FName("C1C04")
	};
}

EItemUseResult ASpotI00::TryUseItem_Implementation(const FName& Name) {
	if (Name == LDConsts::Items::Bra) {
		Diags->AddId("IS_C0_C03");
		return EItemUseResult::BAD_HANDLED;
	}
	return Super::TryUseItem_Implementation(Name);
}
