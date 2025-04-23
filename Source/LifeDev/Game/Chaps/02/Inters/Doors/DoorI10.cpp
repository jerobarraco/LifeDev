// Copyright Jerónimo Barraco-Mármol

#include "DoorI10.h"

#include "Diags/Diags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Core/Consts/ConstItems.h"
#include "Story/Story.h"
// TODO improve this

ADoorI10::ADoorI10():Super() {
	UseAnim = false;
	LockedDlg = "D10_L";

	// no unlock item nor trigger dlg. i want to keep this locked
	IsLocked = true;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CGun(TEXT("/Game/LifeDev/Game/Inters/Generic/Shotgun_Fire_Round_Eject_01.Shotgun_Fire_Round_Eject_01"));
	SFX_Gun = CGun.Object;
}

EItemUseResult ADoorI10::TryUseItem_Implementation(const FName& Name) {
	// shouldn't be triggered with !LD_USE_ITEM
	if (!Interacted && Name == LDConsts::Items::Card1) {
		DoDialog();
		return EItemUseResult::SUCCESS;
	}

	return Super::TryUseItem_Implementation(Name);
}

bool ADoorI10::TryTrigger_Implementation() {
	if (UNLIKELY(!Inventory)) return false;

	// this happens after unlocking the door
	if (Interacted) return Super::TryTrigger_Implementation();
	Interacted = true;

	static const FName Name ("Screwer00");
	if (Inventory->Has(Name)) {
		Inventory->Mod(Name, -1);
		DoDialog();
		return true;
	}

	return Super::TryTrigger_Implementation();
}

void ADoorI10::DoDialog() {
	Diags->OnDone.AddUniqueDynamic(this, &ADoorI10::Shoot);
	Diags->AddId("D10_T"); // after unlocking with the card
}

void ADoorI10::Shoot() {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	Diags->OnDone.RemoveAll(this);

	PlaySFX(SFX_Gun);
	Flashback->SetMin(.4f, .5);
	// TODO use new autodialogs with condition for this.
	LockedDlg = "D10_L.1"; // new dialog from now on
	if (LIKELY(Story)) Story->StartNext();
}