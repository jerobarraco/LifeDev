// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LTeachMan.h"

#include "Kismet/GameplayStatics.h"

#include "Diags/Diags.h"
#include "Interact/CInteractor.h"
#include "Inventory/Inventory.h"
#include "Story/Story.h"

#include "LifeDev/Core/Consts/ConstDlgs.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogLTeachMan, Log, Log)

namespace LifeDev {
	namespace Teach {
		static const FName InterTrigger("Inter.Trigger");
		static const FName ItemPick("Item.Pick");
		static const FName ItemUse("Item.Use");
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

	const ALChar* const Char = ALChar::Instance(this);
	if (UNLIKELY(!Char)) return;

	UCInteractor* const Inter = Char->GetInteractor();
	if (UNLIKELY(!Inter)) return;

	Inter->OnTrigger.AddUniqueDynamic(this, &ALTeachMan::InterTrigger);
	Inter->OnHover.AddUniqueDynamic(this, &ALTeachMan::InterHover);
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
	UInventory* const Items = UInventory::Instance(this);
	if (LIKELY(Items) && !ItemHasAll()) {
		Items->OnMod.AddUniqueDynamic(this, &ALTeachMan::ItemMod);
	}

	UDiags* const Diags = UDiags::Instance(this);
	if (LIKELY(Diags)) {
		Diags->OnAdd.AddUniqueDynamic(this, &ALTeachMan::DiagAdd);
	}

	UStory* const Story = UStory::Instance(this);
	if (LIKELY(Story)) {
		Story->OnStart.AddUniqueDynamic(this, &ALTeachMan::StepStart);
	}
}

void ALTeachMan::DeInitItemMod() {
	UInventory* const Items = UInventory::Instance(this);
	if (LIKELY(Items)) {
		Items->OnMod.RemoveAll(this);
	}
}

void ALTeachMan::ItemMod(const FName& Name, const int32 Diff, const FItem& Item) {
	if (Diff>0) { // when acquiring items
		Show( LifeDev::Teach::ItemPick);
	} else if (Diff<0) {
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

bool ALTeachMan::ItemHasAll() {
	return Has(LifeDev::Teach::ItemPick) && Has(LifeDev::Teach::ItemConsume);
}

void ALTeachMan::DiagAdd(const FName& Name, const FDiag& Diag) {
	if (UNLIKELY(Name.IsNone())) return;
	// a cheeky way to detect events. but i don't care atm.
	const FString& SName = Name.ToString();
	if (SName.StartsWith(LDConsts::Dlgs::Item::LookPre))
		Hide(LifeDev::Teach::ItemPick); // i can dismiss the message here.
	if (SName.StartsWith(LDConsts::Dlgs::Item::UsePre))
		Hide(LifeDev::Teach::ItemUse);
}

void ALTeachMan::StepStart(AStep* const Step) {}

