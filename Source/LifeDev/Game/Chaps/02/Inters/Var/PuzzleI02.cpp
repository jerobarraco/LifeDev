// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI02.h"

#include "Interact/CPuzzle.h"

APuzzleI02::APuzzleI02():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0, 0, 3}; // S.A.D

	static FName DoneId = "PZ02_T";
	DoneDlg = DoneId;
	DoneFB = .15;
}
