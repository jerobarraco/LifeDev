 // Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryUI.h"

#include "LInventoryItemUI.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryItemUI.h"

ULInventoryUI::ULInventoryUI() {
	static ConstructorHelpers::FClassFinder<ULInventoryItemUI>
		CItem(TEXT("/Game/LifeDev/Game/Inventory/UI/W_InvItemUI.W_InvItemUI_C"));
	ItemClass = CItem.Class;
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
	// Super::SetItemMod_Implementation(Name, Diff, Item);

	if (UNLIKELY(Name.IsNone() | !SItems)) return;

	UE_LOG(LogTemp, Log, TEXT("%hs Name=%s Diff=%i Count=%i"), __func__, *Name.ToString(),
		Diff, Item.Count);

	Fade(true);

	TObjectPtr<ULInventoryItemUI>* const pIt = Items.Find(Name);
	ULInventoryItemUI* It = pIt ? pIt->Get() : nullptr;
	if (Item.Count<=0) {
		if (LIKELY(It)) {
			UE_LOG(LogTemp, Log, TEXT("%hs Name=%s Removing"), __func__, *Name.ToString());
			It->Fade(false);
			
			// fade, add timer. then remove.
			FTimerDelegate D;
			D.BindLambda([It, this, Name] {
				Items.Remove(Name);
				SItems->RemoveChild(It);
			});
			FTimerHandle H;
			const UWorld* const World = GetWorld();
			// ideally this shouldn't be hardcoded. but if we overextend nobody is going to die.
			if (LIKELY(World)) World->GetTimerManager().SetTimer(H, D, 1, false);
		}
		return;
	}

	if (!It) {
		It = CreateWidget<ULInventoryItemUI>(this, ItemClass.Get());
		UE_LOG(LogTemp, Log, TEXT("%hs Name=%s Creating p=%p"), __func__,
			*Name.ToString(), It);
		Items.Add(Name, It); // the padding is embedded in the itemui_w itself
		UScrollBoxSlot* const Slot = Cast<UScrollBoxSlot>(SItems->AddChild(It));
		// this is important so the setselected works well
		if (LIKELY(Slot)) Slot->SetVerticalAlignment(VAlign_Bottom);

		It->Fade(true);
	}

	if (UNLIKELY(!It)) return; // safeguard
	It->SetItem(Name, Item);
}

void ULInventoryUI::SetSelected_Implementation(const FName& Name) {
	// Super::SetSelected_Implementation(Name);
	if (UNLIKELY(!SItems)) return; // if name is none it will unselect everything. it's not a good situation, but "i'll allow it!".

	TArray<FName> Keys;
	Items.GetKeys(Keys);
	for (const FName& K: Keys) {
		ULInventoryItemUI* const It = Items[K];
		if (UNLIKELY(!It)) continue;

		const bool Sel = It->Name == Name;
		It->SetSelected(Sel);
		if (!Sel) continue;

		SItems->ScrollWidgetIntoView(It, true);
	}

	Fade(true);
}

void ULInventoryUI::SetItemUsed_Implementation(const FName& Name) {
	// Super::SetItemUsed_Implementation(Name);
	
	// handles name==none. i have to check item after this anyway.
	// this will update the color in case of cooldown, also count.
	UInventoryItemUI* const Item = ResetItem(Name);
	if (LIKELY(Item)) Item->Use(); // animate

	Fade(true);
}


void ULInventoryUI::SetItemCold_Implementation(const FName& Name) {
	// Super::SetItemCold_Implementation(Name);
	UE_LOG(LogTemp, Log, TEXT("%hs Name=%s"), __func__, *Name.ToString());
	ResetItem(Name);
	Fade(true);
}

ULInventoryItemUI* ULInventoryUI::GetItem(const FName& Name) {
	if (UNLIKELY(Name.IsNone())) return nullptr;

	TObjectPtr<ULInventoryItemUI>* const pIt = Items.Find(Name);
	ULInventoryItemUI* const It = pIt ? pIt->Get() : nullptr;
	return It;
}

ULInventoryItemUI* ULInventoryUI::ResetItem(const FName& Name) {
	UE_LOG(LogTemp, Log, TEXT("%hs Name=%s"), __func__, *Name.ToString());
	
	// Super::SetItemCold_Implementation(Name);
	const UInventory* const Inv = UInventory::Instance(this);
	if (UNLIKELY(!Inv)) return nullptr;
	
	ULInventoryItemUI* const It = GetItem(Name);
	FItem Item;
	const bool Ok = Inv->Get(Name, Item);
	if (LIKELY(!!It & Ok)) It->SetItem(Name, Item);

	return It;
}

void ULInventoryUI::Fade_Implementation(const bool In) {}

void ULInventoryUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (UNLIKELY(!SItems)) return;

	SItems->ClearChildren(); // needed since i leave a stub item for helping with layout
}
