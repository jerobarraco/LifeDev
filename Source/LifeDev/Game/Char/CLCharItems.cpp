// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "CLCharItems.h"

#include "Kismet/GameplayStatics.h"

#include "Diags/Diags.h"
#include "Interact/CInteractor.h"
#include "Interact/InteractTypes.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryTypes.h"
#include "Inventory/ItemLogic.h"
#include "JUtils/Misc/JUtilsSys.h"

#include "LifeDev/Core/Consts/ConstDlgs.h"
#include "LifeDev/Core/Settings/LSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharItems, Log, Log);

void UCLCharItems::BeginPlay() {
	Super::BeginPlay();
	Diags = UDiags::Instance(this);
	Inventory = UInventory::Instance(this);
	Flags = UFlags::Instance(this);
	const AActor* const Owner = GetOwner();
	Interactor = Owner ?
		Cast<UCInteractor>(Owner->GetComponentByClass(UCInteractor::StaticClass())) :
		nullptr;
	UE_CLOG(UNLIKELY(!Interactor), LogCharItems, Warning,
		TEXT("%hs Could not obtain the interactor component from the owner."),
		__func__);
}

void UCLCharItems::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Diags = nullptr;
	Inventory = nullptr;
	Interactor = nullptr;
	Flags = nullptr;
	Super::EndPlay(EndPlayReason);
}

bool UCLCharItems::Say(const FName& Name) const {
	return LIKELY(Diags) ? Diags->AddId(Name) : false;
}

void UCLCharItems::Look(const FName& Name) const {
	const FString& SName = *Name.ToString();
	UE_LOG(LogCharItems, Log, TEXT("%hs Name=%s"), __func__, *SName);

	if (UNLIKELY(!IsValid(Inventory))) return;

	if (UNLIKELY(Name.IsNone())) {
		UE_LOG(LogCharItems, Log, TEXT("%hs tried to look at a NONE item."), __func__);
		return;
	}

	FItem Item;
	if (UNLIKELY(!Inventory->Get(Name, Item))) {
		UE_LOG(LogCharItems, Log, TEXT("%hs Can't find the item name='%s'"), __func__, *SName);
		return;
	}
	
	UE_LOG(LogCharItems, Log, TEXT("%hs '%s'. Title='%s' Count=%i, Descriptions=%i."),
		__func__, *SName, *Item.Title.ToString(), Item.Count, Item.Descriptions.Num());
	
	// say look at stuff.
	// don't even bother with the non-random.
	// if you want to have a non-random sequence you'd have to add 2 keys.
	// but it's cheaper than asking every time for random and not random.
	// const FName& DRName = FName((SName + "_Look*")); // TODO deprecated
	const FName& DiagName = FName(LDConsts::Dlgs::Item::LookPre+SName);
	// the isValid is for the add below
	const bool Said = Say(DiagName); // || Say(DRName) ; // notice it calls Say first.
	if (LIKELY(IsValid(Diags)) & !Said) {
		// otherwise compose one
		// show the dialog with the description. this is temporary until i make the ui
		FDiag Diag;
		// Diag.Type = EDialogType::SYSTEM;
		// Diag.Text = Item.Description;
		Diag.CharRow = "Sys";
		Diag.Comment = "look";
		// allow item description to split on different dialog boxes/pages.
		for (const FText& Block: Item.Descriptions) {
			Diag.Text = Block;
			Diags->AddDiag(Diag);
		}
	}

	// trigger manager look
	if (IsValid(Item.Logic)) Item.Logic->Look();
}

EItemUseResult UCLCharItems::Use(const FName& Name) const {
	UE_LOG(LogCharItems, Log, TEXT("%hs Name=%s"), __func__, *Name.ToString());

	if (UNLIKELY(!IsValid(Inventory))) {
		UE_LOG(LogCharItems, Warning, TEXT("%hs Inventory not found. Stop."), __func__);
		return EItemUseResult::ERROR;
	}
	
	if (UNLIKELY(!Interactor)) {
		UE_LOG(LogCharItems, Warning,
			TEXT("%hs Could not obtain the interactor component from the owner."), __func__);
		return EItemUseResult::ERROR;
	}

	FItem Item;
	const bool Found = Inventory->GetSelectedItem(Item);
	if (UNLIKELY(!Found)) {
		UE_LOG(LogCharItems, Warning, TEXT("%hs Item not found '%s'. Stop."),
			__func__, *Name.ToString());
		return EItemUseResult::ERROR;
	}

	if (UNLIKELY(!Inventory->IsCold(Item))) {
		UE_LOG(LogCharItems, Log, TEXT("%hs Item not ready. Skip."), __func__);
		Say(LDConsts::Dlgs::Item::NotReady);
		return EItemUseResult::BAD_HANDLED;
	}

	if (UNLIKELY(!Item.Usable && !DoUse(Name, Item))) {
		UE_LOG(LogCharItems, Log, TEXT("%hs Item not usable. Stop."), __func__);
		const bool Said = Say(LDConsts::Dlgs::Item::NotUsable);
		return EItemUseResult::ERROR; // always return if not usable (why?)
	}

	return EItemUseResult::SUCCESS;
}

bool UCLCharItems::DoUse(const FName Name, const FItem& Item) const {
	UE_LOG(LogCharItems, Log, TEXT("%hs: Name=%s, Item=%s"),
		__func__, *Name.ToString(), *Item.Title.ToString());

	const bool Used = Inventory->Use(Name); //important
	const bool ValidLogic = IsValid(Item.Logic);
	// save myself some pain if i forget. warn to myself.
	UE_CLOG(UNLIKELY(!ValidLogic), LogCharItems, Warning, TEXT("%hs Item has no logic."
		"It won't really be used. unless hooked somewhere else. Skip."), __func__);
	// can't quit now. we've used the item. (yes, i could change the code to accomodate for that, but no.
	if (LIKELY(ValidLogic)) Item.Logic->Use();

	if (UNLIKELY(!Used)) return false;

	const FString& NameS = Name.ToString();
	const FName Dlg(LDConsts::Dlgs::Item::UsePre+NameS);
	Say(Dlg);
	PlaySound(Item.Snd);
	Flags->Mod(Dlg, 1);

	// isDebug is an optimization since the compiler will hopefully remove it . UJUtilsSys::IsDebug() & should i?
	if (ULSettings::GetFeatS(this, EFeat::DBG_FAST_COOL))
		Inventory->SetCool(Name);

	return true;
}

EItemUseResult UCLCharItems::UseSelected() const {
	return Use(Inventory->GetSelected());
}

void UCLCharItems::LookSelected() const {
	if (LIKELY(IsValid(Inventory))) Look(Inventory->GetSelected());
}

bool UCLCharItems::PlaySound(const TSoftObjectPtr<USoundBase>& Snd) const {
	// if (!Snd.GetUniqueID().IsValid()) return false; // do i ever need this?
	if (!Snd.IsValid()) return false;

	const AActor* const Owner = GetOwner();
	UE_LOG(LogCharItems, Log, TEXT("%hs Play sound '%s'."), __func__, *Snd.ToString());
	if (UseSndAtLocation & LIKELY(Owner)) {
		const FVector& Location = Owner->GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(this, Snd.Get(), Location);
	} else
		UGameplayStatics::PlaySound2D(this, Snd.Get());

	return true;
}