// Copyright Jerónimo Barraco-Mármol

#include "DoorI10.h"

#include "Diags/Diags.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ADoorI10::ADoorI10():Super() {
	LockedDlg = "D10_L";
	Locked = true;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CGun(TEXT("//TODO"));//Todo
	SFX_Gun = CGun.Object ;
}

bool ADoorI10::TryTrigger_Implementation() {
	if(!Interacted) {
		PlaySFX(SFX_Gun);
		Flashback->ModVal(.3, .5);
		Dialogs->AddId("D10_Gun");
	}
	Interacted = true;

	return Super::TryTrigger_Implementation();
}

