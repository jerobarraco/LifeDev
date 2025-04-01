// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI09.h"

#include "Interact/CPuzzle.h"

APuzzleI09::APuzzleI09():Super() {
	CPuzzle->Type = EPuzzleType::SEQUENCE;
	CPuzzle->Solution = {9,1,1};
}

