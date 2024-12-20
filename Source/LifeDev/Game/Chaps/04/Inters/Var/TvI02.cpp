// Copyright Jerónimo Barraco-Mármol

#include "TvI02.h"

#include "LifeDev/Core/Consts/ConstItems.h"

static const FName NTriggerDlg = "TV02_T";
ATvI02::ATvI02():Super() {
	UseItemDlgs = {
		{LDConsts::Items::Card0, "TV02xC00"},
		{LDConsts::Items::Card1, "TV02xC01"},
		{LDConsts::Items::Card2, "TV02xC02"},
		{LDConsts::Items::Card3, "TV02xC03"},
		{LDConsts::Items::Card4, "TV02xC04"},
	};
	TriggerDlg = NTriggerDlg;
}

void ATvI02::SetState_Implementation(const int32 NewState) {
	Super::SetState_Implementation(NewState);
	// ==1 because it's set for the next interaction
	// only trigger when turning on
	TriggerDlg = NewState == 1 ? NTriggerDlg : NAME_None;
}
