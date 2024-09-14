// Copyright Jerónimo Barraco-Mármol

#include "SpotI02.h"

#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ASpotI02::ASpotI02():Super() {
	DropDlg = TEXT("IS0_D*");
	TriggerDlg = "IS0_T"; // when completed
	LockedDlg = "IS0_L"; // when not full
	LockedFullDlg = "IS0_LF"; // when full
	DropFullDlg = ""; // when full
	RewardFlag = LDConsts::Flags::CH0::Spot;
	Items = {
		// a bra and a shirt a boxer and a sock
		FName("C1C01"), FName("C1C02"), LDConsts::Items::Bra, FName("C1C04")
	};
	// RewardActor set in editor
}
