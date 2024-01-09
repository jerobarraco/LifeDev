// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI01.h"

#include "Interact/CPuzzle.h"

APuzzleI01::APuzzleI01():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->SolutionIDs = {5,4,0,5}; // A.N.G.E.R. // TODO
	CPuzzle->DisableOnDone = true;

	static FName DoneId = "PZ01_T";
	DoneDlg = DoneId;
	DoneFB = .2;
}
