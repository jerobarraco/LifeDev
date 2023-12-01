// Copyright Jerónimo Barraco-Mármol

#include "LItemLogic.h"

#include "Diags/Diags.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "Story/Story.h"

ULItemLogic::ULItemLogic():Super() {}

void ULItemLogic::BeginPlay_Implementation(UWorld* Trash) {
	UE_LOG(LogTemp, Verbose, TEXT("LItemLogic:%hs"), __func__);

	UWorld* const W = GetWorld();
	Super::BeginPlay_Implementation(W);
	
	if (!IsValid(W)) {
		UE_LOG(LogTemp, Warning, TEXT("LItemLogic:%hs. can't find world"), __func__);
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

void ULItemLogic::BeginDestroy() {
	Diags = nullptr;
	FB = nullptr;
	Story = nullptr;
	Inv = nullptr;
	Flags = nullptr;

	Super::BeginDestroy();
}

void ULItemLogic::Use_Implementation() {
	Super::Use_Implementation(); // いらない. but still prints a nice log.
	Say(UseDlg);
}

bool ULItemLogic::Say(const FName& Id) {
	if (!IsValid(Diags)) return false;
	return Diags->AddId(Id);
}
