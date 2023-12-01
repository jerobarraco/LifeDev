// Copyright Jerónimo Barraco-Mármol

#include "LItemLogic.h"

#include "Diags/Diags.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ULItemLogic::ULItemLogic():Super() {}

void ULItemLogic::BeginPlay_Implementation(UWorld* Trash) {
	UE_LOG(LogTemp, Verbose, TEXT("LItemLogic:%hs"), __func__);

	UWorld* const NewWorld = GetWorld();
	Super::BeginPlay_Implementation(NewWorld);
	
	if (!IsValid(NewWorld)) {
		Diags = nullptr;
		FB = nullptr;
		return;
	}

	Diags = NewWorld->GetSubsystem<UDiags>();
	FB = NewWorld->GetSubsystem<UFlashback>();
}

void ULItemLogic::BeginDestroy() {
	Diags = nullptr;
	FB = nullptr;
	Super::BeginDestroy();
}
