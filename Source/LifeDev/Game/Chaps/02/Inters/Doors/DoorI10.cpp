// Copyright Jerónimo Barraco-Mármol

#include "DoorI10.h"

#include "Diags/Diags.h"
#include "LifeDev/Core/Consts/ConstItems.h"
#include "Story/Story.h"

#include "LifeDev/Game/Flashback/Flashback.h"
// TODO improve this

ADoorI10::ADoorI10():Super() {
	UseAnim = false; // don't animate, stay visually closed.
	IsOneShot = true;
	StateNum = 1;
	// LockedDlg = "D10_L"; // TODo this dialog should change.
	// no unlock item nor trigger dlg. i want to keep this locked
	IsLocked = true;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CGun(TEXT("/Game/LifeDev/Game/Inters/Generic/Shotgun_Fire_Round_Eject_01.Shotgun_Fire_Round_Eject_01"));
	SFX_Gun = CGun.Object;
	UnlockItems = {LDConsts::Items::Key};
}

void ADoorI10::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	DoDialog(); // TODO next sprint.
	// this is currently this way until i implement the item usage on next sprint. maybe the screwer.
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
	// LockedDlg = "D10_L.1"; // new dialog from now on
	if (LIKELY(Story)) Story->StartNext();
}
