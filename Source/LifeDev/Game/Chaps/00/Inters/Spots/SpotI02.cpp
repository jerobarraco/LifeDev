// Copyright Jerónimo Barraco-Mármol

#include "SpotI02.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ASpotI02::ASpotI02():Super() {
	DropDlg = TEXT("IS0_D*");
	TriggerDlg = "IS0_T"; // when completed
	LockedDlg = "IS0_L"; // when not full
	DropFullDlg = ""; // when full
	Items = {
		// a bra and a shirt a boxer and a sock
		FName("C1C01"), FName("C1C02"), LDConsts::Items::Bra, FName("C1C04")
	};
}

void ASpotI02::Trigger_Implementation() {
	Super::Trigger_Implementation();
	
	// Reward here the card
	if (!IsValid(Card)) return;
	Card->SetActorHiddenInGame(false);
}
