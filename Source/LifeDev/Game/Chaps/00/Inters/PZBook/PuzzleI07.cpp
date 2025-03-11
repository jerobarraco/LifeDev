// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI07.h"

#include "Interact/CPuzzle.h"

APuzzleI07::APuzzleI07():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {2, 1, 1};
	RewardFlash = .2;
}

// void APuzzleI07::PostLoad() {
	// by now the interacts set in editor are loaded
	// Super::PostLoad();
	// static const TMap<FName, FName> Dlgs = {
	// 	{LDConsts::Items::Card0, "PZ00xC00"}
	// };
	// SetUseItemDlgs(Dlgs);
	//
	// static const TArray<bool> Locks = { true, false, true, false };
	// SetLocks(Locks);
// }

// no need to change the state. since to the users' eyes is relative.
// in any case i rather change the solution and the seed for the books.
// void APuzzleI07::BeginPlay() {
	// Super::BeginPlay();
	// static const TArray<int32> States = { 0,0,0,0 };
	// SetStates(States);
// }
