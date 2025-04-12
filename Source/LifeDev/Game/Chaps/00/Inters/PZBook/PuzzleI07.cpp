// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI07.h"

#include "Interact/CPuzzle.h"

APuzzleI07::APuzzleI07():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {2, 1, 1};
	RewardFlash = .2;
	UseActiveOnce = true; // so that it can be solved on different chapters
}

// void APuzzleI07::BeginPlay() {
	// Super::BeginPlay();

	// Ok to only check on start. If we're on the right step then go ahead. 
	// const UStory* const Story = UStory::Instance(this);
	// if (UNLIKELY(!Story)) return;
	// const bool Ok = Story->GetCurrent() == "C0S0"; // Only checking for C0S0 since we can't start on c0s1, and all the other story steps don't make a difference.
	// SetActives(Ok);
// }

