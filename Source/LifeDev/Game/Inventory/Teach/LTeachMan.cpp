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
		static const FName FlagFlash("Flag.Flash");
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

void ALTeachMan::Init_Implementation() {
	Super::Init_Implementation();
	
	ULSettings* const Settings = ULSettings::Instance(this);
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

void ALTeachMan::DeInitFeat() {
	UE_LOG(LogLTeachMan, Log, TEXT("%hs"), __func__);
	DeInitInter();
	DeInitItemMod();
	DeInitDiag();
	DeInitStory();

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	World->GetTimerManager().ClearAllTimersForObject(this);
}

void ALTeachMan::DeInit_Implementation() {
	ULSettings* const Settings = ULSettings::Instance(this);
	if (LIKELY(Settings)) Settings->OnFeatUpdateGameplay.RemoveAll(this);
	Items = nullptr;
	Super::DeInit_Implementation();
}

void ALTeachMan::InitDelayed() {
	// this function exists to skip a bunch of event getting triggered during init.
	// as well as a bunch of dead time during load and fades

	// AVOID FLAGS they update too frequently.

	// TODO only bind if necessary. like Items
	// this is because otherwise this class will annoy players during load saved games too,
	// actually not, the base class already solves that. but it's still less efficient since it's waiting for events, that might never happen (like step c1s1) 

	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;

	const ALChar* const Char = ALChar::Instance(this);
	if (UNLIKELY(!Char)) return;

	UCInteractor* const Inter = Char->GetInteractor();
	if (UNLIKELY(!Inter)) return;

	Inter->OnTrigger.AddUniqueDynamic(this, &ALTeachMan::InterTrigger);
	Inter->OnHover.AddUniqueDynamic(this, &ALTeachMan::InterHover);

	if (LIKELY(Items) && !ItemHasAll()) {
		Items->OnMod.AddUniqueDynamic(this, &ALTeachMan::ItemMod);
		Items->OnUsed.AddUniqueDynamic(this, &ALTeachMan::ItemUse);
	}

	UDiags* const Diags = UDiags::Instance(this);
	if (LIKELY(Diags)) {
		Diags->OnAdd.AddUniqueDynamic(this, &ALTeachMan::DiagAdd);
	}

	const ULSettings* const Settings = ULSettings::Instance(this);
	int32 Chapter = -1;
	if (LIKELY(Settings)) {
		Chapter = Settings->CurrentChapter();
		// no need to check on FeatUp. this is only a warning for when you start the game with the flag set.
		// if you turn it on, you should know what you're doing and how to turn it off.
		if (LIKELY(Settings->GetFeat(EFeat::V_STROBE) && !Has(LifeDev::Teach::FlagFlash))) {
			World->GetTimerManager().SetTimer(HFlash, this, &ALTeachMan::TeachFlash, 3, true);
		}
	}

	UStory* const Story = UStory::Instance(this);
	if (bool(Story) & (Chapter < 2) && !Has(LifeDev::Teach::ItemUse)) {
		Story->OnStart.AddUniqueDynamic(this, &ALTeachMan::StepStart);
	}
}

void ALTeachMan::BeginPlay() {
	Super::BeginPlay();
	Items = UInventory::Instance(this);
}

void ALTeachMan::DeInitItemMod() {
	if (LIKELY(Items)) {
		Items->OnMod.RemoveAll(this);
		Items->OnUsed.RemoveAll(this);
	}
}

void ALTeachMan::ItemMod(const FName& Name, const int32 Diff, const FItem& Item) {
	if (Diff>0) { // when acquiring items
		const bool HasItems = LIKELY(Items);
		const int32 NumItems = HasItems ? Items->GetAll().Num() : -1;
		const bool CanChange = NumItems > 1;
		if (HasItems & CanChange) {
			ItemSelCount = 0; // test
			// bind ONLY here. so it doesn't count previous scrolls. as the user might not have realized what he did
			Items->OnSelected.AddUniqueDynamic(this, &ALTeachMan::ItemSel);
			Show(LifeDev::Teach::ItemChange);
		} else
			Show(LifeDev::Teach::ItemPick);
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
	if (LIKELY(ItemSelCount<2)) return;

	Hide(LifeDev::Teach::ItemChange);// not working

	if (LIKELY(Items))
		Items->OnSelected.RemoveAll(this);
}

void ALTeachMan::ItemUse(const FName& Name) {
	FItem Item;
	const bool Ok = LIKELY(Items) ? Items->GetSelectedItem(Item) : false;
	const bool ShouldHide = Ok & Item.SelfUsable;
	if (ShouldHide)
		Hide(LifeDev::Teach::ItemUse);
}

bool ALTeachMan::ItemHasAll() {
	return Has(LifeDev::Teach::ItemPick) && Has(LifeDev::Teach::ItemConsume)
	&& Has(LifeDev::Teach::ItemChange) && Has(LifeDev::Teach::ItemUse);
}

void ALTeachMan::DiagAdd(const FName& Name, const FDiag& Diag) {
	if (UNLIKELY(Name.IsNone())) return;
	// unfortunately this will ONLY trigger if the dialog is ACTUALLY shown
	// a cheeky way to detect events. but i don't care atm.
	const FString& SName = Name.ToString();
	// downside. will only work if the dialog itself uses this format, which not all do.
	// TODO find something better. some objects might not even have a look, but instead use the inventory description.
	if (SName.StartsWith(LDConsts::Dlgs::Item::LookPre) ) {
		Hide(LifeDev::Teach::ItemPick); // i can dismiss the message here.
		DeInitDiag();
	}
}

void ALTeachMan::StepStart(AStep* const Step) {
	if (UNLIKELY(!Step)) return;
	// i want something more optimized, but this will have to do for now.
	if (Step->Name == "C1S1") // the first safe place to tell the user to use the card
		Show(LifeDev::Teach::ItemUse);

	const ULSettings* const Settings = ULSettings::Instance(this);
	if (LIKELY(Settings && Settings->GetFeat(EFeat::V_STROBE))) {
		// this is a critical one because it's a safety issue.
		// hence. i'm going to keep trying to show this until it's shown.
		// todo find a better way to do this.
		Show(LifeDev::Teach::FlagFlash);
	}
}

void ALTeachMan::FeatUp(const EFeat Feat, const bool Enabled) {
	if (LIKELY(Feat != EFeat::G_TEACH)) return;
	// https://www.youtube.com/watch?v=g-WPhYREFjk
	// according to Fedor Pikus this is BAD (most of the time) unfortunately he didn't say way
	// and while i don't know him nor i have put the time to dis/prove it,
	// i think it's well researched and seems true. and there's no reason to complicate this
	// void(ALTeachMan::* X[] )() = {&ALTeachMan::DeInitFeat, &ALTeachMan::InitFeat};
	// (this->*X[Enabled])();
	if (Enabled) { InitFeat(); } else { DeInitFeat(); }
}

void ALTeachMan::TeachFlash() {
	if (UNLIKELY(Has(LifeDev::Teach::FlagFlash))) {
		const UWorld* const World = GetWorld();
		if (UNLIKELY(!World)) return;

		World->GetTimerManager().ClearTimer(HFlash);
		HFlash.Invalidate();
	}

	// not bothering with the return since the timer will retry
	Show(LifeDev::Teach::FlagFlash);
}
