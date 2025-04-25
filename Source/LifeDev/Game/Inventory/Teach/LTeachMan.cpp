// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LTeachMan.h"

#include "Interact/CInteractor.h"
#include "Inventory/Inventory.h"
#include "Kismet/GameplayStatics.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Sys/LGGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogLTeachMan, Log, Log)

namespace LifeDev {
	namespace Teach {
		static const FName InterTrigger("Inter.Trigger");
		static const FName ItemPick("Item.Pick");
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

	const ALChar* const Char = ALChar::Instance(this);
	if (UNLIKELY(!Char)) return;

	UCInteractor* const Inter = Char->GetInteractor();
	if (UNLIKELY(!Inter)) return;

	Inter->OnTrigger.AddUniqueDynamic(this, &ALTeachMan::InterTrigger);
	Inter->OnHover.AddUniqueDynamic(this, &ALTeachMan::InterHover);
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

void ALTeachMan::ItemMod(const FName& Name, const int32 Diff, const FItem& Item) {
	if (Diff<1) return; // only on acquisition
	Show(LifeDev::Teach::ItemPick);
	DeInitItemMod();
}

void ALTeachMan::DeInit_Implementation() {
	DeInitInter();
	DeInitItemMod();
	Super::DeInit_Implementation();
}

void ALTeachMan::InitDelayed() {
	UInventory* const Items = UInventory::Instance(this);
	if (LIKELY(Items)) {
		Items->OnMod.AddUniqueDynamic(this, &ALTeachMan::ItemMod);
	}
}

void ALTeachMan::DeInitItemMod() {
	UInventory* const Items = UInventory::Instance(this);
	if (LIKELY(Items)) {
		Items->OnMod.RemoveAll(this);
	}
}

void ALTeachMan::InterTrigger(const UCInteract* const Comp) {
	Hide(LifeDev::Teach::InterTrigger);
	DeInitInter();
}

void ALTeachMan::InterHover(const bool bOn, UCInteract* const Comp) {
	if (!bOn & !Comp) return;
	// TODO have a timer so that i have to look at it for a few seconnds
	Show(LifeDev::Teach::InterTrigger);
}

