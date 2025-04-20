// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI00.h"

#include "Interact/CPuzzle.h"
#include "LifeDev/Core/Consts/ConstFlags.h"
#include "LifeDev/Core/Consts/ConstItems.h"

APuzzleI00::APuzzleI00():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {5,4,0,5}; // F.E.A.R.

	static FName DoneId = "PZ00_T";
	RewardFlash = .2;
	RewardFlag = LDConsts::Flags::Stats::Puzzles::Cube; // i could move this to a base class for cube puzzles
	TriggerDlg = DoneId;
}

void APuzzleI00::PostLoad() {
	// by now the interacts set in editor are loaded
	Super::PostLoad();
	static const TMap<FName, FName> Dlgs = {
		{LDConsts::Items::Card0, "PZ00xC00"}
	};
	SetUseItemDlgs(Dlgs);

	static const TArray<bool> Locks = { true, false, true, false };
	SetLocks(Locks);
}

void APuzzleI00::BeginPlay() {
	Super::BeginPlay();
	static const TArray<int32> States = { 5,0,0,0 };
	SetStates(States);
	SetActorHiddenInGame(true); // start hidden
}
