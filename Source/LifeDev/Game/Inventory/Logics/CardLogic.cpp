// Copyright Jerónimo Barraco-Mármol

#include "CardLogic.h"

UCardLogic::UCardLogic():Super() {
}

void UCardLogic::Use_Implementation() {
	Super::Use_Implementation();
	UE_LOG(LogTemp, Log, TEXT("Card being used: '%s'"), *Name.ToString());
}
