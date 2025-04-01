// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI08.h"

#include "Interact/CPuzzle.h"
#include "Story/Story.h"

APuzzleI08::APuzzleI08():Super() {
	CPuzzle->Type = EPuzzleType::SEQUENCE;
	CPuzzle->Solution = {9,1,1};
}

void APuzzleI08::BeginPlay() {
	Super::BeginPlay();
	UStory* const Story = UStory::Instance(this);
	if (UNLIKELY(!Story)) return;
	Story->OnStart.AddUniqueDynamic(this, &APuzzleI08::SetStep);
}

void APuzzleI08::SetStep(AStep* const Step) {
	if (UNLIKELY(!Step)) return;
	SetActives(false);
	if (LIKELY(Step->Name != "C0S0")) return;
	SetActives(false);
}

//
// void APuzzleI08::BeginPlay() {
// 	Super::BeginPlay();
// 	static const TArray<int32> States = { 5,0,0,0 };
// 	SetStates(States);
// }
