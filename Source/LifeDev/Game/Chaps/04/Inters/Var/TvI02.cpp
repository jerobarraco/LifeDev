// Copyright Jerónimo Barraco-Mármol

#include "TvI02.h"

#include "Diags/Diags.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/LGGameMode.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ATvI02::ATvI02():Super() {
	UseItemDlgs = {
		{LDConsts::Items::Card0, "TV02xC00"},
		{LDConsts::Items::Card1, "TV02xC01"},
		{LDConsts::Items::Card2, "TV02xC02"},
		{LDConsts::Items::Card3, "TV02xC03"},
	};
}

void ATvI02::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
}
