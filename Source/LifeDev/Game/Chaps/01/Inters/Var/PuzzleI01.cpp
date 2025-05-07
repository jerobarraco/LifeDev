// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI01.h"

#include "Interact/CInteract.h"
#include "Interact/CPuzzle.h"
#include "LifeDev/Core/Consts/ConstFlags.h"

APuzzleI01::APuzzleI01():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0,1,0,4,5}; // A.N.G.E.R.
	CPuzzle->DisableOnDone = true;

	static FName DoneId = "PZ01_T";
	RewardFlash = .2;
	RewardFlag = LDConsts::Flags::Stats::Puzzles::Cube; // i could move this to a base class for cube puzzles
	TriggerDlg = DoneId;
	UseHint = true;
	Interact->SetProfileHinted();
	UseAutoActivate = true;
}

void APuzzleI01::PostLoad() {
	Super::PostLoad();
	static const TArray<bool> Locks = { true, false, true, true, false };
	SetLocks(Locks);
}

void APuzzleI01::BeginPlay() {
	Super::BeginPlay();

	// 2nd number and last are just random variations
	static const TArray<int32> States = { 0, 4, 0, 4, 2 };
	SetStates(States);
}
