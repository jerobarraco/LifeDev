// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI00.h"

#include "Diags/Diags.h"
#include "Interact/CPuzzle.h"

APuzzleI00::APuzzleI00():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->SolutionIDs = {5,4,0,5}; // F.E.A.R.
	CPuzzle->DisableOnDone = true;

	static FName DoneId = "PZ00_T";
	DoneDlg = DoneId;
}
