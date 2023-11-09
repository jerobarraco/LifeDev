// Copyright Jerónimo Barraco-Mármol

#include "DoorI05.h"

#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"
#include "LifeDev/Game/Sys/Consts/ConstFlags.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"
#include "Story/Story.h"

ADoorI05::ADoorI05():Super() {
	// LockedDlg = "D05_LCK"; // TODO it's not locked. but will be .
	Locked = true;
	AnimEnabled = false; // will take us directly to the next story step
	// Anim->TEnd.SetRotation(FRotator(0, -90, 0).Quaternion());
}

void ADoorI05::BeginPlay() {
	Super::BeginPlay();
	// TODO This is debug. since i haven´t implemented it yet. will do later
	Flags->Mod(LDConsts::Flags::CH0::Leak, 1);
}

bool ADoorI05::TryTrigger_Implementation() {
	// verify the state. (pull not push, just like agile).
	if (!Flags->IsSet(LDConsts::Flags::CH0::Leak)) {
		LockedDlg = "D05_L.Leak";
	} else if (!Flags->IsSet(LDConsts::Flags::CH0::SpotI02)) {
		LockedDlg = "D05_L.Spot";
	} else if (!Inventory->Has(LDConsts::Items::Card0)) {
		LockedDlg = "D05_L.C0";
	} else {
		Locked = false;
	}
	
	return Super::TryTrigger_Implementation();
}

void ADoorI05::Trigger_Implementation() {
	static FName Step("C0S0");
	Super::Trigger_Implementation();

	UStory* const Story = GetWorld()->GetSubsystem<UStory>();
	if (!IsValid(Story)) return;
	Story->Stop(Step);
}
