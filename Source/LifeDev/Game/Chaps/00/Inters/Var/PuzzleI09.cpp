// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI09.h"

#include "Interact/CPuzzle.h"
#include "Story/Step.h"
#include "Story/Story.h"

APuzzleI09::APuzzleI09():Super() {
	CPuzzle->Type = EPuzzleType::SEQUENCE;
	CPuzzle->Solution = {9,1,1};
	IsOneShot = true;
	UseActiveOnce = true;
	ResetOnFail = true;
	// idea: change the solution depending on the chapter.
	// will also require to reset when progressing.
}

void APuzzleI09::BeginPlay() {
	Super::BeginPlay();
	if (UNLIKELY(!Story)) return;
	Story->OnStart.AddUniqueDynamic(this, &APuzzleI09::StartStep);
	
	SetActives(false);
	StartStep(Story->GetStep(Story->GetCurrent())); // necessary. the step is already started on beginplay. since the step loads the dl, and this is on a dl.
}

// attempt to make this interaction chapter agnostic
// this is kind of a test. TODO move this class to Game/CHaps/All/Inters/Var/Phone if i decide to keep it like this.
// TODo enable with the story step instead of this
// todo it's better to have multiple phones than having this

void APuzzleI09::StartStep(AStep* const Step) {
	if (UNLIKELY(!Step)) return;
	// only important if i want to trigger on different chapters.
	if (Step->Label == "C0S0") {
		SetActives(true);
		CPuzzle->Solution = {9,1,1};
	}
}
