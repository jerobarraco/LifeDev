// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "CLCharItems.h"

#include "Diags/Diags.h"
#include "Interact/CInteractor.h"
#include "Interact/InteractTypes.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryTypes.h"
#include "Inventory/ItemLogic.h"
#include "LifeDev/Game/Sys/Consts/ConstDlgs.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharItems, Log, Log);

void UCLCharItems::BeginPlay() {
	Super::BeginPlay();
	Diags = UDiags::Instance(this);
	Inventory = UInventory::Instance(GetWorld());

	const AActor* const Owner = GetOwner();
	Interactor = Owner ?
		Cast<UCInteractor>(Owner->GetComponentByClass(UCInteractor::StaticClass())) :
		nullptr;
	if (!Interactor)
		UE_LOG(LogCharItems, Warning,
			TEXT("%hs Could not obtain the interactor component from the owner."),
			__func__);
}

void UCLCharItems::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Diags = nullptr;
	Inventory = nullptr;
	Super::EndPlay(EndPlayReason);
}

bool UCLCharItems::Say(const FName& Name) const {
	if (!IsValid(Diags)) return false;
	return Diags->AddId(Name);
}

void UCLCharItems::Look(const FName& Name) const {
	const FString& SName = *Name.ToString();
	UE_LOG(LogCharItems, Log, TEXT("%hs Name=%s"), __func__, *SName);

	if (!IsValid(Inventory)) return;

	if (Name.IsNone()) {
		UE_LOG(LogCharItems, Log, TEXT("%hs tried to look at a NONE item."), __func__);
		return;
	}

	FItem Item;
	if (!Inventory->Get(Name, Item)) {
		UE_LOG(LogCharItems, Log, TEXT("%hs Can´t find the item name='%s'"), __func__, *SName);
		return;
	}
	
	UE_LOG(LogCharItems, Log, TEXT("%hs '%s'. Title='%s' Count=%i, description '%s'."),
		__func__, *SName, *Item.Title.ToString(), Item.Count, *Item.Description.ToString());

	// say look at stuff.
	// don't even bother with the non-random.
	// if you want to have a non-random sequence you'd have to add 2 keys.
	// but it's cheaper than asking every time for random and not random.
	const FName& DRName = FName(*(SName + "_Look*"));
	// the isValid is for the add below
	if (!Say(DRName) && IsValid(Diags)) { // notice it calls Says
		// otherwise compose one
		// show the dialog with the description. this is temporary until i make the ui
		FDialog Diag;
		Diag.Type = EDialogType::SYSTEM;
		Diag.Text = Item.Description;
		Diag.CharRow = "Sys";
		Diags->AddDiag(Diag);
	}

	// trigger manager look
	if (IsValid(Item.Logic)) Item.Logic->Look();
}

EItemUseResult UCLCharItems::Use(const FName& Name) const {
	UE_LOG(LogCharItems, Log, TEXT("%hs Name=%s"), __func__, *Name.ToString());

	if (!IsValid(Inventory)) return	EItemUseResult::ERROR;
	
	FItem Item;
	const bool Found = Inventory->GetSelectedItem(Item);
	if (!Found) return EItemUseResult::ERROR;

	if (!Item.Usable) {
		UE_LOG(LogCharItems, Log, TEXT("%hs Item not usable"), __func__);
		Say(LDConsts::Dlgs::Sys::Item::NotUsable);
		return EItemUseResult::ERROR; // always return if not usable
	}

	if (!Inventory->IsCold(Item)) {
		UE_LOG(LogCharItems, Log, TEXT("%hs Item not ready"), __func__);
		Say(LDConsts::Dlgs::Sys::Item::NotReady);
		return EItemUseResult::ERROR;
	}

	if (!Interactor) {
		UE_LOG(LogCharItems, Warning,
			TEXT("%hs Could not obtain the interactor component from the owner."), __func__);
		return EItemUseResult::ERROR;
	}
	
	// this will try trigger the item. i can show dialogs there if i need to.
	// though maybe it would be nice to have something generic as well.
	const EItemUseResult Res = Interactor->TryUseItem(Name);
	if (Res == EItemUseResult::BAD_HANDLED) {
		UE_LOG(LogCharItems, Log, TEXT("%hs Can't use item with that. But it was handled."),
			__func__);
		return Res;
	}

	if (Item.SelfUsable) {
		// notice only checking auto-trigger here. so that i can use an auto trigger with an interact too.
		// (notice this if is separate from the one above)
		UE_LOG(LogCharItems, Log, TEXT("%hs Item is self-usable. will attempt now. '%s'."),
			__func__, *Item.Title.ToString());

		if (IsValid(Item.Logic)) Item.Logic->Use();
		// don't return here.
	} else if (Res != EItemUseResult::SUCCESS) { // notice bad handled above returns, and this is else.
		const bool isBadTarget = Res == EItemUseResult::BAD_TARGET;
		UE_LOG(LogCharItems, Log, TEXT("%hs Can't use item with that. res=%s '%s' badTarget=%i"),
			__func__, *UEnum::GetValueAsString(Res), *Item.Title.ToString(), isBadTarget);
		const FName& DlgId = isBadTarget ?
			LDConsts::Dlgs::Sys::Item::BadTarget :
			LDConsts::Dlgs::Sys::Item::NoTarget;
		Say(DlgId);
		return Res;
	}

	// mark the item as used, it won't trigger the manager.
	// since we don't want to trigger when is used with an interaction.
	Inventory->Use(Name);
	return EItemUseResult::SUCCESS;
}

EItemUseResult UCLCharItems::UseSelected() const {
	return IsValid(Inventory) ? Use(Inventory->GetSelected()) :  EItemUseResult::ERROR;
}

void UCLCharItems::LookSelected() const {
	if (IsValid(Inventory)) Look(Inventory->GetSelected());
}
