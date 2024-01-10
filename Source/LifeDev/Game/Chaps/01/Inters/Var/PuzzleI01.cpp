// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI01.h"

#include "Interact/CPuzzle.h"

APuzzleI01::APuzzleI01():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0,1,0,4,5}; // A.N.G.E.R.
	CPuzzle->DisableOnDone = true;

	static FName DoneId = "PZ01_T";
	DoneDlg = DoneId;
	DoneFB = .2;
}
