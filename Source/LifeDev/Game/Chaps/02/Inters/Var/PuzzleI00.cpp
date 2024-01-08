// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI00.h"

#include "Diags/Diags.h"
#include "Interact/CPuzzle.h"

APuzzleI00::APuzzleI00():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->SolutionIDs = {5,4,0,5}; // F.E.A.R.
	CPuzzle->DisableOnDone = true;
}

void APuzzleI00::Done_Implementation(bool IsOk) {
	Super::Done_Implementation(IsOk);
	static FName N = "PZ00_T";
	Diags->AddId(N);
}
