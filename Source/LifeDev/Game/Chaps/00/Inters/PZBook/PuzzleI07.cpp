// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI07.h"

#include "Interact/CPuzzle.h"

APuzzleI07::APuzzleI07():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {2, 1, 1}; // TODO

	static FName DoneId = "PZ00_T";
	DoneDlg = DoneId;
	DoneFB = .2;
	// DoneFlag = ; LDConsts::Flags::Stats::Puzzles::Cube; // TODO
}

void APuzzleI07::PostLoad() {
	// by now the interacts set in editor are loaded
	Super::PostLoad();
	// static const TMap<FName, FName> Dlgs = {
	// 	{LDConsts::Items::Card0, "PZ00xC00"}
	// };
	// SetUseItemDlgs(Dlgs);
	//
	// static const TArray<bool> Locks = { true, false, true, false };
	// SetLocks(Locks);
}

void APuzzleI07::BeginPlay() {
	Super::BeginPlay();
	// TODO
	static const TArray<int32> States = { 5,0,0,0 };
	SetStates(States);
}
