 // Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryUI.h"

#include "LInventoryItemUI.h"
#include "Components/ScrollBox.h"
#include "Inventory/InventoryItemUI.h"

ULInventoryUI::ULInventoryUI() {
	static ConstructorHelpers::FClassFinder<ULInventoryItemUI>
		CItem(TEXT("/Game/LifeDev/Game/Inventory/UI/W_InvItemUI.W_InvItemUI_C"));
	ItemClass = CItem.Class;
}

void ULInventoryUI::FadeUsed(const bool Fwd) { // todo move to uinventoryui
	if (UNLIKELY(!AUsed)) return;

	PlayAnimation(AUsed, 0, 1,
		Fwd ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse, 1);
}

void ULInventoryUI::Show_Implementation() {
	PreShow();
	Super::Show_Implementation();
}

void ULInventoryUI::Hide_Implementation() {
	Super::Hide_Implementation();
	PostHide();
}

void ULInventoryUI::SetItemMod_Implementation(const FName& Name, const int32 Diff, const FItem& Item) {
	Super::SetItemMod_Implementation(Name, Diff, Item);

	if (UNLIKELY(Name.IsNone() | !SItems)) return;

	UE_LOG(LogTemp, Log, TEXT("%hs Name=%s Diff=%i Count=%i"), __func__, *Name.ToString(),
		Diff, Item.Count);

	TObjectPtr<ULInventoryItemUI>* const pIt = Items.Find(Name);
	ULInventoryItemUI* It = pIt ? pIt->Get() : nullptr;
	if (Item.Count<=0) {
		if (LIKELY(It)) {
			UE_LOG(LogTemp, Log, TEXT("%hs Name=%s Removing"), __func__, *Name.ToString());
			Items.Remove(Name);
			SItems->RemoveChild(It);
		}
		// todo fade, add timer. then remove.
		return;
	}
	
	if (!It) {
		It = CreateWidget<ULInventoryItemUI>(this, ItemClass.Get());
		UE_LOG(LogTemp, Log, TEXT("%hs Name=%s Creating p=%p"), __func__,
			*Name.ToString(), It);
		Items.Add(Name, It);
		SItems->AddChild(It);
		It->Fade(true);
		// todo fade
	}

	if (UNLIKELY(!It)) return; // safeguard
	It->SetItem(Name, Item);
}
