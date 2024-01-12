// Copyright Jerónimo Barraco-Mármol

#include "PuzzleI01.h"

#include "Interact/CPuzzle.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

APuzzleI01::APuzzleI01():Super() {
	CPuzzle->Type = EPuzzleType::COMBINATION;
	CPuzzle->Solution = {0,1,0,4,5}; // A.N.G.E.R.
	CPuzzle->DisableOnDone = true;

	static FName DoneId = "PZ01_T";
	DoneDlg = DoneId;
	DoneFB = .2;
}

void APuzzleI01::PostLoad() {
	Super::PostLoad();
	if (!CPuzzle) return;

	static const TMap<FName, FName> Dlgs = {
        {LDConsts::Items::Card0, "PZ01xC00"},
        {LDConsts::Items::Card1, "PZ01xC01"},
    };
    SetUseItemDlgs(Dlgs);
}
