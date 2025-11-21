 // Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryUI.h"

#include "LInventoryItemUI.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryItemUI.h"

DEFINE_LOG_CATEGORY_STATIC(LogLInventoryUI, Log, Log)
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

	UE_LOG(LogLInventoryUI, Log, TEXT("%hs Name=%s Diff=%i Count=%i"), __func__, *Name.ToString(),
		Diff, Item.Count);

	Fade(true);

	TObjectPtr<ULInventoryItemUI>* const pIt = Items.Find(Name);
	ULInventoryItemUI* It = pIt ? pIt->Get() : nullptr;
	if (Item.Count<=0) {
		if (LIKELY(It)) {
			UE_LOG(LogLInventoryUI, Log, TEXT("%hs Name=%s Removing"), __func__, *Name.ToString());
			It->Fade(false);
			
			// fade, add timer. then remove.
			FTimerDelegate D;
			D.BindLambda([It, this, Name] {
				// there's a potential bug here, where you loose an item and pick it very quickly
				// this could get executed after the item has been picked up.
				// it's a very strange edge case, but i rather cover it now.
				const UInventory* const Inv = UInventory::Instance(this);
				if (LIKELY(Inv)) {
					FItem NIt;
					if (UNLIKELY(Inv->Get(Name, NIt) || NIt.Count >0)) { // notice shortcut ||
						UE_LOG(LogLInventoryUI, Log,
							TEXT("%hs Item re-added before the remove animation was done. Crisis averted."),
							__func__, *Name.ToString());
						// In theory, the IT is shared since we get and store in the SItems.
						// so we need to show it again.
						It->Fade(true);
						return; // don't remove
					}
				}
				
				Items.Remove(Name);
				SItems->RemoveChild(It); // remove by pointer, order could have changed
			});
			FTimerHandle H;
			const UWorld* const World = GetWorld();
			// ideally this shouldn't be hardcoded. but if we overextend nobody is going to die.
			if (LIKELY(World)) World->GetTimerManager().SetTimer(H, D, 1, false);
			// seems to be necessary. unfortunately the Keys in the inventory gets reordered when an item gets removed. (wtf)
			// and i won't have an extra array to fix that (for now at least)
			// so we need to reorder on removal. there's also reorder on add, so it's ok if an item gets readded. 
			ReorderItems();
		}
		return;
	}

	bool Created = false;
	if (!It) {
		It = CreateWidget<ULInventoryItemUI>(this, ItemClass.Get());
		UE_LOG(LogLInventoryUI, Log, TEXT("%hs Name=%s Creating p=%p"), __func__,
			*Name.ToString(), It);
		Items.Add(Name, It); // the padding is embedded in the itemui_w itself
		
		AddItem(It);

		It->Fade(true);
		Created = true;
	}

	if (UNLIKELY(!It)) return; // safeguard
	It->SetItem(Name, Item);

	if (UNLIKELY(Created)) ReorderItems(); // this adds quite some processing so let's gate it.
}

void ULInventoryUI::AddItem(UWidget* const It) const {
	// "Slot" is a class member, don't shadow or windows will cry.
	UScrollBoxSlot* const pSlot = Cast<UScrollBoxSlot>(SItems->AddChild(It));
	// this is important so the SetSelected works well
	if (LIKELY(pSlot)) pSlot->SetVerticalAlignment(VAlign_Bottom);
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
	UE_LOG(LogLInventoryUI, Log, TEXT("%hs Name=%s"), __func__, *Name.ToString());
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
	UE_LOG(LogLInventoryUI, Log, TEXT("%hs Name=%s"), __func__, *Name.ToString());
	
	// Super::SetItemCold_Implementation(Name);
	const UInventory* const Inv = UInventory::Instance(this);
	if (UNLIKELY(!Inv)) return nullptr;
	
	ULInventoryItemUI* const It = GetItem(Name);
	FItem Item;
	const bool Ok = Inv->Get(Name, Item);
	if (LIKELY(bool(It) & Ok)) It->SetItem(Name, Item);

	return It;
}

void ULInventoryUI::Fade_Implementation(const bool In) {}

void ULInventoryUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (UNLIKELY(!SItems)) return;

	SItems->ClearChildren(); // needed since i leave a stub item for helping with layout
	Items.Empty();
}

void ULInventoryUI::NativeDestruct() {
	// very important when reloading the level after an item use.
	const UWorld* const World = GetWorld();
	if (LIKELY(World)) World->GetTimerManager().ClearAllTimersForObject(this);
	
	Super::NativeDestruct();
	
}

void ULInventoryUI::ReorderItems() {
	UE_LOG(LogLInventoryUI, Log, TEXT("%hs"), __func__);
	const UInventory* const Inv = UInventory::Instance(this);
	if (UNLIKELY(!Inv)) return;

	const TMap<FName, FItem>& All = Inv->GetAll();
	TArray<FName> Keys;
	All.GetKeys(Keys);
	const int32 Num = Keys.Num();
	for (int32 i = 0; i<Num; ++i) {
		const FName& K = Keys[i];
		ULInventoryItemUI* const IUI = GetItem(K);
		UE_LOG(LogLInventoryUI, Verbose, TEXT("%hs moving item n=%s p=%llu i=%i"),
			__func__, *K.ToString(), IUI, i);
		if (UNLIKELY(!IUI)) {
			UE_LOG(LogLInventoryUI, Warning,
				TEXT("%hs Could not find the widget for the specified item. N=%s"),
				__func__, *K.ToString());
			continue;
		}
		// ok so this is not working. i'll do it manually
		// SItems->ShiftChild(i, IUI); // Thanks, Tim's team. amazing addition.
		// this works because i'm removing basically all items (at some point)
		// and adding them in order (since i'm iterating by the key array)
		// so in the end they'll end up in order
		// if there's an item that is not in the keys array, it will end up first.
		// this happens when removing an item. it's a shame. but... i also don't want to overcomplicate this
		// it's probably less efficient than ShiftChild too
		SItems->RemoveChild(IUI);
		AddItem(IUI);
	}
}
