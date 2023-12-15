// Copyright Jerónimo Barraco-Mármol

#include "TapeI01.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "Story/Story.h"

ATapeI01::ATapeI01():Super() {
	Texts = { FText::FromString(TEXT("Pick up tape")) };
	TriggerDlg = FName("T0_T"); // will say what it is. triggered on pick up
	StateNum = 1;
	UseAnim = false;
	RewardFlash = .1;
	// RewardItem = "T00";
	// needed to be able to attach to the drawer
	Super::SetMobility(EComponentMobility::Movable);
}

void ATapeI01::Trigger_Implementation() {
	Super::Trigger_Implementation();
	UStory* const Story = UStory::Get(GetWorld());
	if (!Story) return;
	Story->StartNextStep("C1S1");
	
	AnimFade->OnEnd.AddUniqueDynamic(this, &ATapeI01::K2_DestroyActor);
	Fade(false);
}
