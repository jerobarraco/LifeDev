// Copyright Jerónimo Barraco-Mármol

#include "CardMan.h"

UCardMan::UCardMan():Super() {
}

void UCardMan::Use_Implementation() {
	Super::Use_Implementation();
	UE_LOG(LogTemp, Log, TEXT("Card being used: '%s'"), *Name.ToString());
}
