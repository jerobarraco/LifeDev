// Copyright Jerónimo Barraco-Mármol

#include "DoorI05.h"

#include "Story/Story.h"

ADoorI05::ADoorI05():Super() {
	LockedDlg = "D05_LCK";
	Locked = false; // TODO need to add an interaction to unlock
	AnimEnabled = false; // will take us directly to the next story step 
}

void ADoorI05::Trigger_Implementation() {
	static FName Step("C0S0");
	Super::Trigger_Implementation();
	UStory* const Story = GetWorld()->GetSubsystem<UStory>();
	if (!IsValid(Story)) return;
	Story->Stop(Step);
}
