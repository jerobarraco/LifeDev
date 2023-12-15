// Copyright Jerónimo Barraco-Mármol

#include "DoorI10.h"

#include "Diags/Diags.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "Story/Story.h"

ADoorI10::ADoorI10():Super() {
	LockedDlg = "D10_L";
	Locked = true;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CGun(TEXT(""));//Todo
	SFX_Gun = CGun.Object;
}

void ADoorI10::Shoot() {
	PlaySFX(SFX_Gun);
	Flashback->ModVal(.3, .5);
	Dialogs->AddId("D10_Gun");
	LockedDlg = "D10_L.1";
	UStory::Get(GetWorld())->StartNextStep("C2S0");
}

bool ADoorI10::TryTrigger_Implementation() {
	if(!Interacted) {
		FTimerHandle H;
		GetWorld()->GetTimerManager().SetTimer(H, this, &ADoorI10::Shoot, 2);
		Shoot();
	}
	Interacted = true;

	return Super::TryTrigger_Implementation();
}

