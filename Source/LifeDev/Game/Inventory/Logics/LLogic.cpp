// Copyright Jerónimo Barraco-Mármol

#include "LLogic.h"

#include "Diags/Diags.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "Story/Story.h"

ULLogic::ULLogic():Super() {}

void ULLogic::BeginPlay_Implementation(UWorld* Trash) {
	UE_LOG(LogTemp, Verbose, TEXT("LItemLogic:%hs"), __func__);

	UWorld* const W = GetWorld();
	Super::BeginPlay_Implementation(W);
	
	if (!IsValid(W)) {
		// this actually happens on editor. understandable since the begin play is fake.
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

bool ULLogic::Say(const FName& Id) {
	if (!IsValid(Diags)) return false;
	return Diags->AddId(Id);
}
