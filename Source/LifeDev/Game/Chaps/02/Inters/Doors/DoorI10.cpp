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

bool ADoorI10::TryTrigger_Implementation() {
	if(!Interacted) {
		FTimerHandle H;
		UWorld* const World = GetWorld();
		if (!World) return false;
		World->GetTimerManager().SetTimer(H, this, &ADoorI10::Shoot, 3);
	}
	Interacted = true;

	return Super::TryTrigger_Implementation();
}

void ADoorI10::Shoot() {
	UWorld* const W = GetWorld();
	if (!W) return;

	PlaySFX(SFX_Gun);
	if (Flashback->GetVal()<.5) {
		Flashback->SetVal(.5, 1);
	}
	Dialogs->OnDone.AddUniqueDynamic(this, &ADoorI10::AfterShot);
	
	Dialogs->AddId("D10_Gun");
	LockedDlg = "D10_L.1"; // new dialog from now on
}

void ADoorI10::AfterShot() {
	Dialogs->OnDone.RemoveAll(this);
	UWorld* const W = GetWorld();
	if (!W) return;
	UStory::Get(W)->StartNext("C2S0");
}

