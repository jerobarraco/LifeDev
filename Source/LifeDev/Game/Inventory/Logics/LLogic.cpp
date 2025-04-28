// Copyright Jerónimo Barraco-Mármol

#include "LLogic.h"

#include "Diags/Diags.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "Story/Story.h"

ULLogic::ULLogic():Super() {}

void ULLogic::BeginPlay_Implementation() {
	UE_LOG(LogTemp, Verbose, TEXT("LLogic:%hs"), __func__);

	Super::BeginPlay_Implementation();
	
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!IsValid(W))) {
		Diags = nullptr;
		FB = nullptr;
		Story = nullptr;
		Inv = nullptr;
		Flags = nullptr;
		return;
	}

	Diags = W->GetSubsystem<UDiags>();
	FB = W->GetSubsystem<UFlashback>();
	Story = W->GetSubsystem<UStory>();
	Inv = W->GetSubsystem<UInventory>();
	Flags = W->GetSubsystem<UFlags>();
}

void ULLogic::BeginDestroy() {
	Diags = nullptr;
	FB = nullptr;
	Story = nullptr;
	Inv = nullptr;
	Flags = nullptr;

	Super::BeginDestroy();
}

void ULLogic::Use_Implementation() {
	Super::Use_Implementation(); // いらない. but still prints a nice log.
	Say(UseDlg);
}

bool ULLogic::Say(const FName& Id) const {
	return LIKELY(IsValid(Diags)) ? Diags->AddId(Id) : false;
}
