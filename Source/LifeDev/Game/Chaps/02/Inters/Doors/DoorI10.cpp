// Copyright Jerónimo Barraco-Mármol

#include "DoorI10.h"

#include "Diags/Diags.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"
#include "Story/Story.h"

ADoorI10::ADoorI10():Super() {
	LockedDlg = "D10_L";
	// no unlock item nor trigger dlg. i want to keep this locked
	Locked = true;
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CGun(TEXT(""));//Todo
	SFX_Gun = CGun.Object;
}


EItemUseResult ADoorI10::TryUseItem_Implementation(const FName& Name) {
	if (!Interacted && Name == LDConsts::Items::Card2) {
		DoDialog();
		return EItemUseResult::SUCCESS;
	}

	return Super::TryUseItem_Implementation(Name);
}

void ADoorI10::DoDialog() {
	// this happens after unlocking the door with the card
	if(Interacted) return;
	Interacted = true;

	Dialogs->OnDone.AddUniqueDynamic(this, &ADoorI10::Shoot);
	Dialogs->AddId("D10_T"); // after unlocking with the card
}

void ADoorI10::Shoot() {
	UWorld* const W = GetWorld();
	if (!W) return;

	Dialogs->OnDone.RemoveAll(this);

	PlaySFX(SFX_Gun);
	Flashback->SetMin(.4f, .5);
	LockedDlg = "D10_L.1"; // new dialog from now on
	UStory::Get(W)->StartNext("C2S0");
}