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
		SItems->AddChild(It);
		It->Fade(true);
	}

	if (UNLIKELY(!It)) return; // safeguard
	It->SetItem(Name, Item);
}

void ULInventoryUI::SetItemUsed_Implementation(const FName& Name) {
	// Super::SetItemUsed_Implementation(Name);
	
	UInventoryItemUI* const Item = GetItem(Name);
	if (LIKELY(Item)) Item->Use();
}

ULInventoryItemUI* ULInventoryUI::GetItem(const FName& Name) {
	if (UNLIKELY(Name.IsNone())) return nullptr;

	TObjectPtr<ULInventoryItemUI>* const pIt = Items.Find(Name);
	ULInventoryItemUI* const It = pIt ? pIt->Get() : nullptr;
	return It;
}

void ULInventoryUI::NativeOnInitialized() {
	Super::NativeOnInitialized();
	if (UNLIKELY(!SItems)) return;

	SItems->ClearChildren(); // needed since i leave a stub item for helping with layout
}
