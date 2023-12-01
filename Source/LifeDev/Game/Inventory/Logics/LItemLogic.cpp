// Copyright Jerónimo Barraco-Mármol

#include "LItemLogic.h"

#include "Diags/Diags.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ULItemLogic::ULItemLogic():Super() {}

void ULItemLogic::Use_Implementation() {
	Super::Use_Implementation();
	UE_LOG(LogTemp, Log, TEXT("Item being used: '%s'"), *Name.ToString());
}

void ULItemLogic::BeginPlay_Implementation(UWorld* NewWorld) {
	UE_LOG(LogTemp, Log, TEXT("LItemLogic:%hs"), __func__);

	Super::BeginPlay_Implementation(NewWorld);
	if (!IsValid(NewWorld)) return;
	Diags = NewWorld->GetSubsystem<UDiags>();
	FB = NewWorld->GetSubsystem<UFlashback>();
}

void ULItemLogic::BeginDestroy() {
	Diags = nullptr;
	FB = nullptr;
	Super::BeginDestroy();
}
