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
#include "LifeDev/Core/Settings/LSettingsUI.h"
#include "LifeDev/Game/Char/CLCharItems.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

// note. the way that this class obtains information from the rest of the game
// and interacts with objects, it's not my preferred way of doing things.
// it might look a bit hackish. and that's fine.
// the idea is that this class should impact the rest of the code as much as possible.
// as if it were a late addition.
// but i'll try to do it as cleanly as possible (without modifying the rest of the code to accomodate for this).

DEFINE_LOG_CATEGORY_STATIC(LogLTeachMan, Log, Log)

namespace LD {
	namespace Teach {
		static const FName InterTrigger("Inter.Trigger");
		static const FName ItemPick("Item.Pick");
		static const FName ItemUse("Item.Use");
		static const FName ItemChange("Item.Change");
		static const FName ItemConsume("Item.Consume");
		static const FName FlagFlash("Flag.Flash");
		static const FName GameSetting("Game.Setting");
	}
}

ALTeachMan* ALTeachMan::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const World = O->GetWorld();
	if (UNLIKELY(!IsValid(World))) return nullptr;

	const ALGGameMode* const Mode = Cast<ALGGameMode>(World->GetAuthGameMode());
	if (LIKELY(IsValid(Mode)))
		return Mode->TeachMan;

	return Cast<ALTeachMan>(UGameplayStatics::GetActorOfClass(World, ALTeachMan::StaticClass()));
}

void ALTeachMan::Init_Implementation() {
	Super::Init_Implementation();
	
	Settings = ULSettings::Instance(this);
	if (LIKELY(Settings)) {
		Settings->OnFeatUpdateGameplay.AddUniqueDynamic(this, &ALTeachMan::FeatUp);
		if (UNLIKELY(!Settings->GetFeat(EFeat::G_TEACH))) return; // don't do initfeat if i don't have the feat.
	}

	InitFeat();
}

void ALTeachMan::InitFeat() {
	UE_LOG(LogLTeachMan, Log, TEXT("%hs"), __func__);
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	FTimerHandle H;
	// this should actually wait for the step start. not going to do that atm.
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

void ALTeachMan::DeInitLook() {
	const ALChar* const Char = ALChar::Instance(this);
	if (UNLIKELY(!Char)) return;

	UCLCharItems* const Items = Char->GetCharItems();
	if (UNLIKELY(!Items)) return;

	Items->OnLook.RemoveAll(this);
}

void ALTeachMan::DeInitStory() {
	if (LIKELY(Story)) Story->OnStart.RemoveAll(this);
	Story = nullptr;
}

void ALTeachMan::DeInitFeat() {
	UE_LOG(LogLTeachMan, Log, TEXT("%hs"), __func__);
	DeInitInter();
	DeInitItemMod();
	DeInitLook();
	DeInitStory();

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	World->GetTimerManager().ClearAllTimersForObject(this);
}

void ALTeachMan::DeInit_Implementation() {
	if (LIKELY(Settings)) Settings->OnFeatUpdateGameplay.RemoveAll(this);
	Settings = nullptr;
	Items = nullptr;
	Super::DeInit_Implementation();
}

void ALTeachMan::InitDelayed() {
	// this function exists to skip a bunch of event getting triggered during init.
	// as well as a bunch of dead time during load and fades

	// AVOID FLAGS they update too frequently.

	// only bind if necessary. like Items
	// this is because otherwise this class will annoy players during load saved games too,
	// actually not, the base class already solves that. but it's still less efficient since it's waiting for events, that might never happen (like step c1s1) 

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	const ALChar* const Char = ALChar::Instance(this);
	if (UNLIKELY(!Char)) return;

	SettingsUI = Char->GetSettingsUI();
	if (LIKELY(SettingsUI)) {
		SettingsUI->OnDone.AddUniqueDynamic(this, &ALTeachMan::SettingsDone);
	} else
		UE_LOG(LogLTeachMan, Warning, TEXT("%hs SettingsUI not found."), __func__);

	UCInteractor* const Inter = Char->GetInteractor();
	if (LIKELY(Inter)) {
		Inter->OnTrigger.AddUniqueDynamic(this, &ALTeachMan::InterTrigger);
		Inter->OnHover.AddUniqueDynamic(this, &ALTeachMan::InterHover);
	}

	if (LIKELY(Items) & UNLIKELY(!ItemHasAll())) {
		Items->OnMod.AddUniqueDynamic(this, &ALTeachMan::ItemMod);
		Items->OnUsed.AddUniqueDynamic(this, &ALTeachMan::ItemUse);
	}

	Diags = UDiags::Instance(this);
	if (UNLIKELY(!Has(LD::Teach::ItemPick))) {
		UCLCharItems* const Items = Char->GetCharItems();
		if (LIKELY(Items))
			Items->OnLook.AddUniqueDynamic(this, &ALTeachMan::ItemLook);
	}

	int32 Chapter = -1;
	if (LIKELY(Settings)) {
		Chapter = Settings->CurrentChapter();
		// no need to check on FeatUp. this is only a warning for when you start the game with the flag set.
		// if you turn it on, you should know what you're doing and how to turn it off.
		if (LIKELY(Settings->GetFeat(EFeat::V_STROBE) && !Has(LD::Teach::FlagFlash))) {
			World->GetTimerManager().SetTimer(HFlash, this, &ALTeachMan::FlashTeach,
				3, true);
			OnHide.AddUniqueDynamic(this, &ALTeachMan::FlagHide);
		}
	}

	Story = UStory::Instance(this);
	if (LIKELY(bool(Story) & (Chapter < 2)) && !Has(LD::Teach::ItemUse))
		Story->OnStart.AddUniqueDynamic(this, &ALTeachMan::StepStart);
}

void ALTeachMan::BeginPlay() {
	Super::BeginPlay();
	Items = UInventory::Instance(this);
}

bool ALTeachMan::Show_Implementation(const FName& Id) {
	// don't show a hint if the diags are showing
	if (LIKELY(Diags) & UNLIKELY(Diags->GetIsShowing())) return false;

	return Super::Show_Implementation(Id);
}

void ALTeachMan::DeInitItemMod() const {
	if (LIKELY(Items)) {
		Items->OnMod.RemoveAll(this);
		Items->OnUsed.RemoveAll(this);
	}
}

void ALTeachMan::ItemMod(const FName& Name, const int32 Diff, const FItem& Item) {
	if (Diff>0) { // when acquiring items
		const bool HasItems = LIKELY(Items);
		const int32 NumItems = HasItems ? Items->GetAll().Num() : -1;
		const bool CanSelect = NumItems > 1;
		if (HasItems & CanSelect) {
			ItemSelCount = 0; // reset
			// bind ONLY here. so it doesn't count previous scrolls. as the user might not have realized what he did
			Items->OnSelected.AddUniqueDynamic(this, &ALTeachMan::ItemSel);
			Show(LD::Teach::ItemChange);
		} else
			Show(LD::Teach::ItemPick);
	} else if (Diff<0) {
		if (LIKELY(Item.Consumable)) // don't trigger on cards
			Show(LD::Teach::ItemConsume); // item consumed
	}

	if (UNLIKELY(ItemHasAll()))
		DeInitItemMod(); // not necessary anymore. opt
}

void ALTeachMan::InterTrigger(const UCInteract* const Comp) {
	Hide(LD::Teach::InterTrigger);
	DeInitInter();
}

void ALTeachMan::InterHover(const bool bOn, UCInteract* const Comp) {
	if (!bOn & !Comp) return;
	// TODO have a timer so that i have to look at it for a few seconds
	Show(LD::Teach::InterTrigger);
}

void ALTeachMan::ItemSel(const FName& Name) {
	++ItemSelCount;
	if (LIKELY(ItemSelCount<2)) return;

	Hide(LD::Teach::ItemChange);

	if (LIKELY(Items)) Items->OnSelected.RemoveAll(this);
}

void ALTeachMan::ItemUse(const FName& Name) {
	FItem Item;
	const bool Ok = LIKELY(Items) ? Items->GetSelectedItem(Item) : false;
	const bool ShouldHide = Ok & Item.Usable;
	if (ShouldHide)
		Hide(LD::Teach::ItemUse);
}

bool ALTeachMan::ItemHasAll() const {
	return Has(LD::Teach::ItemPick) && Has(LD::Teach::ItemConsume)
	&& Has(LD::Teach::ItemChange) && Has(LD::Teach::ItemUse);
}

void ALTeachMan::ItemLook(const FName& Name) {
	if (UNLIKELY(Name.IsNone())) return;
	Hide(LD::Teach::ItemPick); // i can dismiss the message here.
	DeInitLook();
}

void ALTeachMan::StepStart(AStep* const Step) {
	if (UNLIKELY(!Step)) return;
	// i want something more optimized, but this will have to do for now.
	if (Step->Name == "C1S1")
		// this step is the first safe place to tell the user to use the card
		Show(LD::Teach::ItemUse);
	else 
		// not really a good place. there's a chance that it could appear during dialogs.
		Show(LD::Teach::GameSetting);
}

void ALTeachMan::FeatUp(const EFeat Feat, const bool Enabled) {
	if (LIKELY(Feat != EFeat::G_TEACH)) return;
	// https://www.youtube.com/watch?v=g-WPhYREFjk
	// according to Fedor Pikus this is BAD (most of the time) unfortunately he didn't say why
	// and while i don't know him nor i have put the time to dis/prove it,
	// i think it's well researched and seems true. and there's no reason to complicate this
	// void(ALTeachMan::* X[] )() = {&ALTeachMan::DeInitFeat, &ALTeachMan::InitFeat};
	// (this->*X[Enabled])();
	if (Enabled) { InitFeat(); } else { DeInitFeat(); }
}

void ALTeachMan::FlashTeach() {
	// this is a critical one because it's a safety issue.
	// hence. i'm going to keep trying to show this until it's shown.
	Show(LD::Teach::FlagFlash);
}

void ALTeachMan::FlagHide(const FName& Id) {
	// this fixes the case in which the dialog is dismissed by timeout
	if (UNLIKELY(Has(LD::Teach::FlagFlash))) FlashDone();
}

void ALTeachMan::FlashDone() {
	UE_LOG(LogLTeachMan, Log, TEXT("%hs"), __func__);
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	World->GetTimerManager().ClearTimer(HFlash);
	HFlash.Invalidate();
	OnHide.RemoveAll(this); // before hide to avoid calling itself
	Hide(LD::Teach::FlagFlash);
}

void ALTeachMan::SettingsDone() {
	UE_LOG(LogLTeachMan, Log, TEXT("%hs"), __func__);
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	// this is to ensure we don't unbind before the flash is shown if it's necessary.
	bool AllDone = LIKELY(Settings) ? !Settings->GetFeat(EFeat::V_STROBE) : true;
	Hide(LD::Teach::GameSetting); // always hide since we come from there.
	// only hide if it was shown. important since it's a health thing.
	const bool FlashShown = Has(LD::Teach::FlagFlash);
	if (FlashShown) {
		FlashDone();
		AllDone = true;
	}

	if (LIKELY(SettingsUI) & AllDone) {
		SettingsUI->OnDone.RemoveAll(this);
		SettingsUI = nullptr; // don't need it anymore. (but i need to make sure i check for it in code)
	}
}
