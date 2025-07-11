// Copyright Jerónimo Barraco-Mármol

#include "DoorI10.h"

#include "Diags/Diags.h"
#include "Story/Story.h"

#include "LifeDev/Game/Flashback/Flashback.h"
// TODO improve this

ADoorI10::ADoorI10():Super() {
	UseAnim = false; // don't animate, stay visually closed.
	IsOneShot = true;
	StateNum = 1;
	// no unlock item nor trigger dlg. i want to keep this locked
	IsLocked = true;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CGun(TEXT("/Game/LifeDev/Game/Inters/Generic/Shotgun_Fire_Round_Eject_01.Shotgun_Fire_Round_Eject_01"));
	SFX_Gun = CGun.Object;
	UnlockItems = {"Screwer00"};
	UseHint = true;
	// using condition instead of reward, since "locked" doesn't reward. (though you can think it rewards the flag)
	HintCondition = "{Inter.Locked.DoorI12}"; // once the door to go out has been triggered. hint this one.
}

void ADoorI10::DoTrigger_Implementation() {
	Diags->OnDone.AddUniqueDynamic(this, &ADoorI10::Shoot);
	Super::DoTrigger_Implementation();
	// this is currently this way until i implement the item usage on next sprint. maybe the screwer.
}

void ADoorI10::Shoot() {
	const UWorld* const W = GetWorld();
	if (UNLIKELY(!W)) return;

	Diags->OnDone.RemoveAll(this);

	PlaySFX(SFX_Gun);
	Flashback->SetMin(.4f, .5);
	if (LIKELY(Story)) Story->StartNext();
}
