// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LTeachMan.h"

#include "Kismet/GameplayStatics.h"

#include "Diags/Diags.h"
#include "Interact/CInteractor.h"
#include "Inventory/Inventory.h"
#include "Story/Story.h"
#include "Story/Step.h"

#include "LifeDev/Core/Consts/ConstDlgs.h"
#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogLTeachMan, Log, Log)

namespace LifeDev {
	namespace Teach {
		static const FName InterTrigger("Inter.Trigger");
		static const FName ItemPick("Item.Pick");
		static const FName ItemUse("Item.Use");
		static const FName ItemChange("Item.Change");
		static const FName ItemConsume("Item.Consume");
	}
}

ALTeachMan* ALTeachMan::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const World = O->GetWorld();
	if (UNLIKELY(!IsValid(World))) return nullptr;

	const ALGGameMode* Mode = Cast<ALGGameMode>(World->GetAuthGameMode());
	if (LIKELY(IsValid(Mode)))
		return Mode->TeachMan;

	return Cast<ALTeachMan>(UGameplayStatics::GetActorOfClass(World, ALTeachMan::StaticClass()));
}

void ALTeachMan::Init_Implementation(UDataTable* Data) {
	Super::Init_Implementation(Data);

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	FTimerHandle H;
	// this should actually wait for the step start. not gonna do that atm.
	World->GetTimerManager().SetTimer(H, this, &ALTeachMan::InitDelayed, InitDelayTime);
}

void ALTeachMan::DeInitInter() {
	const ALChar* const Char = ALChar::Instance(this);
	if (UNLIKELY(!Char)) return;

	UCInteractor* const Inter = Cast<UCInteractor>(Char->GetComponentByClass(UCInteractor::StaticClass()));
	if (UNLIKELY(!Inter)) return;
	Inter->OnTrigger.RemoveAll(this);
	Inter->OnHover.RemoveAll(this);
}

void ALTeachMan::DeInitDiag() {
	UDiags* const Diags = UDiags::Instance(this);
	if (LIKELY(Diags)) {
		Diags->OnAdd.RemoveAll(this);
	}
}

void ALTeachMan::DeInitStory() {
	UStory* const Story = UStory::Instance(this);
	if (LIKELY(Story)) {
		Story->OnStart.RemoveAll(this);
	}
}

void ALTeachMan::DeInit_Implementation() {
	DeInitInter();
	DeInitItemMod();
	DeInitDiag();
	DeInitStory();
	Super::DeInit_Implementation();
}

void ALTeachMan::InitDelayed() {
	// this function exists to skip a bunch of event getting triggered during init.
	// as well as a bunch of dead time during load and fades

	// AVOID FLAGS they update too frequently.

	// TODO only bind if necessary. like Items
	// this is because otherwise this class will annoy players during load saved games too,
	// actually not, the base class already solves that. but it's still less efficient since it's waiting for events, that might never happen (like step c1s1) 

	const ALChar* const Char = ALChar::Instance(this);
	if (UNLIKELY(!Char)) return;

	UCInteractor* const Inter = Char->GetInteractor();
	if (UNLIKELY(!Inter)) return;

	Inter->OnTrigger.AddUniqueDynamic(this, &ALTeachMan::InterTrigger);
	Inter->OnHover.AddUniqueDynamic(this, &ALTeachMan::InterHover);

	UInventory* const Items = UInventory::Instance(this);
	if (LIKELY(Items) && !ItemHasAll()) {
		Items->OnMod.AddUniqueDynamic(this, &ALTeachMan::ItemMod);
		Items->OnSelected.AddUniqueDynamic(this, &ALTeachMan::ItemSel);
	}

	UDiags* const Diags = UDiags::Instance(this);
	if (LIKELY(Diags)) {
		Diags->OnAdd.AddUniqueDynamic(this, &ALTeachMan::DiagAdd);
	}

	const ULSettings* const Settings = ULSettings::Instance(this);
	const int32 Chapter = Settings->CurrentChapter();
	UStory* const Story = UStory::Instance(this);
	if (Chapter < 2 && !!Story & !Has(LifeDev::Teach::ItemUse)) {
		Story->OnStart.AddUniqueDynamic(this, &ALTeachMan::StepStart);
	}
}

void ALTeachMan::DeInitItemMod() {
	UInventory* const Items = UInventory::Instance(this);
	if (LIKELY(Items)) {
		Items->OnMod.RemoveAll(this);
		Items->OnSelected.RemoveAll(this);
	}
}

void ALTeachMan::ItemMod(const FName& Name, const int32 Diff, const FItem& Item) {
	if (Diff>0) { // when acquiring items
		const UInventory* const Items = UInventory::Instance(this);
		const int32 NumItems = LIKELY(Items) ? Items->GetAll().Num() : -1;
		const FName& Id = NumItems > 1 ? LifeDev::Teach::ItemChange: LifeDev::Teach::ItemPick;
		Show(Id);
	} else if (Diff<0) {
		if (LIKELY(Item.Consumable)) // don't trigger on cards
			Show(LifeDev::Teach::ItemConsume); // item consumed
	}

	if (UNLIKELY(ItemHasAll()))
		DeInitItemMod(); // not necessary anymore. opt
}

void ALTeachMan::InterTrigger(const UCInteract* const Comp) {
	Hide(LifeDev::Teach::InterTrigger);
	DeInitInter();
}

void ALTeachMan::InterHover(const bool bOn, UCInteract* const Comp) {
	if (!bOn & !Comp) return;
	// TODO have a timer so that i have to look at it for a few seconds
	Show(LifeDev::Teach::InterTrigger);
}

void ALTeachMan::ItemSel(const FName& Name) {
	++ItemSelCount;
	if (ItemSelCount>2)
		Hide(LifeDev::Teach::ItemChange);// not working

	UInventory* const Items = UInventory::Instance(this);
	if (LIKELY(Items))
		Items->OnSelected.RemoveAll(this);
}

bool ALTeachMan::ItemHasAll() {
	return Has(LifeDev::Teach::ItemPick) && Has(LifeDev::Teach::ItemConsume)
	&& Has(LifeDev::Teach::ItemChange);
}

void ALTeachMan::DiagAdd(const FName& Name, const FDiag& Diag) {
	if (UNLIKELY(Name.IsNone())) return;
	// unfortunately this will ONLY trigger if the dialog is ACTUALLY shown
	// a cheeky way to detect events. but i don't care atm.
	const FString& SName = Name.ToString();
	if (SName.StartsWith(LDConsts::Dlgs::Item::LookPre))
		Hide(LifeDev::Teach::ItemPick); // i can dismiss the message here.
	if (SName.StartsWith(LDConsts::Dlgs::Item::UsePre))
		Hide(LifeDev::Teach::ItemUse); // not working
}

void ALTeachMan::StepStart(AStep* const Step) {
	if (UNLIKELY(!Step)) return;
	// i want something more optimized, but this will have to do for now.
	if (Step->Name == "C1S1") // the first safe place to tell the user to use the card
		Show(LifeDev::Teach::ItemUse);
}

