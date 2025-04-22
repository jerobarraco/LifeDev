// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI02.h"

#include "Interact/CPuzzle.h"
#include "LifeDev/Core/Consts/ConstFlags.h"

APuzzleI02::APuzzleI02():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0, 0, 3}; // S.A.D.

	static FName DoneId = "PZ02_T";
	RewardFlash = .15;
	RewardFlag = LDConsts::Flags::Stats::Puzzles::Cube;
	TriggerDlg = DoneId;
}

void APuzzleI02::PostLoad() {
	Super::PostLoad();

	static const TArray<bool> Locks = {true, false, false};
	SetLocks(Locks);
}

void APuzzleI02::BeginPlay() {
	Super::BeginPlay();

	static const TArray<int32> States = {0, 2, 5};
	SetStates(States);
}


