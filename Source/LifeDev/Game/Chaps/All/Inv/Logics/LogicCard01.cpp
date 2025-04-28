// Copyright Jerónimo Barraco-Mármol

#include "LogicCard01.h"

ULogicCard01::ULogicCard01() {
	Entries.Reserve(1);
	Entries = {
		{.From = {}, .To ={}}
	}; // TODO create defaults
	// TODO at some point have a data table?
}

void ULogicCard01::Use_Implementation() {
	Super::Use_Implementation();
	// find a group of items, replace with another
}
